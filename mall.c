#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <aio.h>
#include <string.h>

#include "helper.h"
#include "test/testRunner.h"

// number of pages taken up by the PageHeaderList
#define HEADER_NUM_PAGES 4 

// size of prefix pointer in bytes (see my_malloc())
#define ALLOC_PREFIX_SIZE 0

/**
 * Singelton structure representing the process' list of PageHeader's.
 * 
 * List is stored in the first [HEADER_NUM_PAGES] pages of the
 * process-initialised heap.
*/
typedef struct PageHeaderList {
    struct PageHeader *head;
    int numPagesAllocated;
    int pageSize;
} PageHeaderList;
static struct PageHeaderList pageHeaderList = {NULL, 0, 0};

/**
 * Represents a single page allocated on the heap.
 * 
 * Pages are allocated in sections of N pages, where N >= 1.
 * 
 * Pages that are first in their section store a non-zero
 * value for numPagesInSection (see below).
*/
typedef struct PageHeader {
    // 1 if page un-mapped, 0 otherwise
    int isFree;

    // pointer to page on heap
    void *pagePtr;

    // number of pages in allocated section (0 if non-first page)
    int numPagesInSection;

    // next page
    struct PageHeader *next;

} PageHeader;

int initialise_page_header_list() {
    if (pageHeaderList.head || pageHeaderList.numPagesAllocated) {
        perror("Heap header already initialised");
        return 1;
    }

    int pageSize = sys_page_size();
    if (!pageSize) {
        return 1;
    }

    // allocate pages to store our PageHeaderList
    PageHeader *head = sbrk(HEADER_NUM_PAGES * pageSize);
    if (head == (void*)-1) {
        perror("Error allocating PageHeaderList");
        return 1;
    }

    pageHeaderList.head = head;
    pageHeaderList.numPagesAllocated = 0;
    pageHeaderList.pageSize = pageSize;

    int numHeaders = (HEADER_NUM_PAGES * pageSize) / sizeof(PageHeader);

    // initialise entire PageHeaderList
    PageHeader *currHeader = pageHeaderList.head;
    int i = 0;
    while (i < numHeaders) {
        currHeader->isFree = 1;
        currHeader->pagePtr = NULL; // pages themselves are lazily allocated when needed
        currHeader->numPagesInSection = 0;
        currHeader->next = currHeader + 1;

        currHeader = currHeader->next;
        i++;
    }
    return 0;
}

void display_page_header_list(int limit) {
    printf("Number of pages allocated: %d\n", pageHeaderList.numPagesAllocated);
    PageHeader *curr = pageHeaderList.head;
    int i = 0;
    printf("----------\n");
    while (curr && i < limit) {
        printf("Page: %d\n", i);
        printf("Page header addr: %p\n", curr);
        printf("Page free: %d\n", curr->isFree);
        printf("Number of pages in section: %d\n", curr->numPagesInSection);
        printf("Page addr: %p\n", curr->pagePtr);
        printf("----------\n");
        curr = curr->next;
        i++;
    }
    return;
}

void* allocate_contiguous_section(PageHeader *firstHeader, int numPages) {
    PageHeader *prevHeader = NULL;
    PageHeader *currHeader = firstHeader;
    int i = 0;
    while (i < numPages) {

        // page already allocated
        if (currHeader->pagePtr) {
            continue;
        }

        // allocate new page
        void *newPagePtr = sbrk(pageHeaderList.pageSize);
        if (newPagePtr == (void*)-1) {
            perror("Error allocating heap page");
            return NULL;
        }

        // ensure new page is contiguous with previous
        if (prevHeader && newPagePtr != (prevHeader->pagePtr + pageHeaderList.pageSize)) {
            perror("New page not contiguous with previous");
            return NULL;
        }

        currHeader->isFree = 0;
        currHeader->pagePtr = newPagePtr;

        prevHeader = currHeader;
        currHeader = currHeader->next;
        i++;
    }

    firstHeader->numPagesInSection = numPages;
    return firstHeader->pagePtr;
}

void* my_malloc(int numBytes) {
    if (numBytes == 0) {
        return NULL;
    }

    // initialise PageHeaderList
    if (pageHeaderList.head == NULL && initialise_page_header_list()) {
        return NULL;
    }

    int numPages = ceil_int((numBytes + ALLOC_PREFIX_SIZE), pageHeaderList.pageSize);
    PageHeader *currHeader = pageHeaderList.head;
    PageHeader *searchHeader;

    // search for contiguous section of [numPages] page headers
    int i = 0;
    while (currHeader) {
        searchHeader = currHeader;
        while (searchHeader && searchHeader->isFree && i < numPages) {
            i++;
            searchHeader = searchHeader->next;
        }

        // successful
        if (i == numPages) {
            void* sectionPtr = allocate_contiguous_section(currHeader, numPages);
            if (sectionPtr) {
                pageHeaderList.numPagesAllocated += numPages;
                return sectionPtr;
            }
        }

        // keep searching
        if (searchHeader) {
            currHeader = searchHeader->next;
        }
    }

    // un-successful
    return NULL;
}

// int my_free(void *ptr) {
//     return 0;

// }

int main() {
    runAll();
    // display_page_header_list(10);
    return 0;
}
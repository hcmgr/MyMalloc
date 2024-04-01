#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <aio.h>
#include <string.h>

#include "mall.h"
#include "helper.h"
#include "test/testRunner.h"

/**
 * Singelton PageHeaderList to store the process' list of PageHeader's
 * 
 * List is stored in the first [HEADER_NUM_PAGES] pages of the
 * process-initialised heap.
*/
static struct PageHeaderList pageHeaderList = {NULL, 0, 0};

/**
 * Returns the process' singleton PageHeaderList
*/
PageHeaderList* get_page_header_list() {
    return &pageHeaderList;
}

/**
 * Initialises the process' singleton PageHeaderList
*/
int initialise_page_header_list() {
    // PageHeaderList already initialised
    if (pageHeaderList.head || pageHeaderList.numPagesAllocated) {
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

/**
 * Displays the first [limit] PageHeaders from the PageHeaderList
*/
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

/**
 * Allocates [numPages] contiguous pages of memory.
 * 
 * Returns a pointer to the first page (at offset 0) on success, NULL otherwise.
*/
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

/**
 * Allocates [numBytes] bytes and returns a pointer to the allocated memory.
 * 
 * Returns a pointer to the allocated memory on success, NULL otherwise.
 * 
 * Allocated memory is page-aligned as per sysconf(_SC_PAGESIZE), and the
 * minimum allocation unit is 1 page.
*/
void* my_malloc(int numBytes) {
    if (numBytes == 0) {
        return NULL;
    }

    // initialise PageHeaderList
    if (pageHeaderList.head == NULL && initialise_page_header_list()) {
        return NULL;
    }

    int numPages = ceil_div((numBytes + ALLOC_PREFIX_SIZE), pageHeaderList.pageSize);
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

                // first 8 bytes reservered for pointer back to pageHeader
                memcpy(sectionPtr, &currHeader, sizeof(PageHeader*));
                return sectionPtr + sizeof(PageHeader*);
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

/**
 * Frees the memory space pointed to by ptr, which must have 
 * been returned by a previous call to my_malloc().
 */
int my_free(void *ptr) {
    // retreive header pointer
    void* rawPtr = ptr - sizeof(PageHeader*);
    PageHeader* firstHeader = *((PageHeader**)rawPtr);
    
    // ensure phPtr is a valid PageHeader pointer
    // TODO: probs more validation needed
    if (firstHeader->numPagesInSection <= 0) {
        perror("Not head of a page section");
        return 1;
    }

    // free all pages in the section
    PageHeader* currHeader = firstHeader;
    int i = currHeader->numPagesInSection;
    while (i > 0 && currHeader->next)  {
        currHeader->isFree = 1;
        memset(currHeader->pagePtr, 0, pageHeaderList.pageSize);

        currHeader = currHeader->next;
        i--;
    }

    if (i > 0) {
        perror("Couldn't free all pages");
        return 1;
    }

    firstHeader->numPagesInSection = 0;
    return 0;

}

int main() {
    runAll();
    // display_page_header_list(10);
    return 0;
}
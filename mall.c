#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <aio.h>
#include <string.h>

#include "helper.h"

// number of pages allocated to store our PageHeader list
#define HEADER_NUM_PAGES 4 

/**
 * Singelton structure holding the linked list of PageHeader's.
 * 
 * List is stored in the first 2 pages of the process-initialised heap.
*/
typedef struct HeapHeader {
    struct PageHeader *head;
    int numPages;
} HeapHeader;
static struct HeapHeader heapHeader = {NULL, 0};

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

    // number of pages in allocated section (0 if non-first page)
    int numPagesInSection;

    // next page
    struct PageHeader *next;

    // pointer to page on heap
    void *pagePtr;

} PageHeader;

int initialise_heap_header() {
    if (heapHeader.head || heapHeader.numPages) {
        perror("Heap header already initialised");
        return 1;
    }

    int pageSize = sys_page_size();
    if (!pageSize) {
        return 1;
    }

    // allocate pages to store our PageHeader list
    PageHeader *firstHeader = sbrk(HEADER_NUM_PAGES * pageSize);
    if (firstHeader == (void*)-1) {
        perror("Error allocating first page");
        return 1;
    }

    int numPages = (HEADER_NUM_PAGES * pageSize) / (sizeof(PageHeader));
    heapHeader.head = firstHeader;
    heapHeader.numPages = numPages;

    // allocate heap pages themselves
    void* currPage = sbrk(numPages * pageSize);

    // initialise all pages and corresponding page headers
    PageHeader *currHeader = heapHeader.head;
    int i = 0;
    while (i < numPages) {
        currHeader->isFree = 1;
        currHeader->numPagesInSection = 1;
        currHeader->next = currHeader + 1;
        currHeader->pagePtr = currPage;

        memset(currPage, 0, pageSize);

        currHeader = currHeader->next;
        currPage = currPage + pageSize;

        i++;
    }
    return 0;
}

int main() {
    initialise_heap_header();
    return 0;
}

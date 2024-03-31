#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <aio.h>
#include "helper.h"

/**
 * Singelton structure holding the linked list of PageHeader's.
 * 
 * List is stored in the first 2 pages of the process-initialised heap.
*/
typedef struct HeapHeader {
    struct PageHeader *head;
    int numPageHeaders;
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

} PageHeader;

int initialise_heap_header() {
    if (heapHeader.head || heapHeader.numPageHeaders) {
        perror("Heap header already initialised\n");
        return 1;
    }

    int pageSize = sys_page_size();
    if (!pageSize) {
        return 1;
    }

    // allocate 2 pages for the list of PageHeader's
    PageHeader *first = sbrk(2 * pageSize);
    if (first == (void*)-1) {
        perror("Error allocating first page");
        return 1;
    }

    int numPageHeaders = (2 * pageSize) / (sizeof(PageHeader));
    heapHeader.head = first;
    heapHeader.numPageHeaders = numPageHeaders;
    
    // initialise all pages as free
    PageHeader *curr = heapHeader.head;
    int i = 0;
    while (i < numPageHeaders) {
        curr->isFree = 1;
        curr->numPagesInSection = 1;
        curr->next = curr + 1;

        curr = curr->next;
        i++;
    }
    return 0;
}

int main() {
    initialise_heap_header();
    return 0;
}

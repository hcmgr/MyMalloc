#ifndef MALL_H
#define MALL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// number of pages taken up by the PageHeaderList
#define HEADER_NUM_PAGES 4 

// size of prefix pointer in bytes (see my_malloc())
#define ALLOC_PREFIX_SIZE 8

/**
 * Singleton PageHeaderList to store the process' list of PageHeader's
 * 
 * List is stored in the first [HEADER_NUM_PAGES] pages of the
 * process-initialized heap.
 */
typedef struct PageHeaderList {
    struct PageHeader *head;
    int numPagesAllocated;
    int pageSize;
} PageHeaderList;

/**
 * Structure representing a single page allocated on the heap.
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

/**
 * Initialises the process' singleton PageHeaderList
*/
int initialise_page_header_list();

/**
 * Returns the process' singleton PageHeaderList
 */
PageHeaderList* get_page_header_list();

/**
 * Displays the first [limit] PageHeaders from the PageHeaderList
*/
void display_page_header_list(int limit);

/**
 * Allocates [numBytes] bytes and returns a pointer to the allocated memory.
 * 
 * Returns a pointer to the allocated memory on success, NULL otherwise.
 * 
 * Allocated memory is page-aligned as per sysconf(_SC_PAGESIZE), and the
 * minimum allocation unit is 1 page.
 */
void* my_malloc(int numBytes);

/**
 * Frees the memory space pointed to by ptr, which must have 
 * been returned by a previous call to my_malloc().
 */
int my_free(void *ptr);

#endif /* MALL_H */

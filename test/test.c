/**
 * All tests written in this file
*/

#include <stdlib.h>
#include <stdio.h>

#include "testHelpers.h"
#include "../helper.h"
#include "../mall.h"

void setup() {
    initialise_page_header_list();
}

void test_zero_alloc() {
    int *ptr = (int *)my_malloc(0);
    if (ptr != NULL) {
        my_free(ptr);
        print_green("Failed\n");
        return;
    }
    print_green("Passed!\n");
}

void test_simple_alloc() {
    int *ptr = (int *)my_malloc(sizeof(int));
    if (ptr == NULL) {
        my_free(ptr);
        print_green("Failed\n");
        return;
    }
    ptr[0] = 1;
    if (ptr[0] != 1) {
        my_free(ptr);
        print_green("Failed\n");
        return;
    }
    my_free(ptr);
    print_green("Passed!\n");
}

void test_multi_page_alloc() {
    PageHeaderList* phl = get_page_header_list();
    int numPages = 3;
    void *ptr = my_malloc(phl->pageSize * numPages);

    // expect 1 more page due to a non-zero ALLOC prefix
    if (ALLOC_PREFIX_SIZE) {
        numPages++;
    }
    
    if (phl->numPagesAllocated != numPages) {
        my_free(ptr);
        print_red("Failed\n");
        return;    
    }

    my_free(ptr);
    print_green("Passed!\n");
}

void test_arrays_example() {
    PageHeaderList* phl = get_page_header_list();
    int len = phl->pageSize - ALLOC_PREFIX_SIZE;
    int* a = my_malloc(len * sizeof(int));
    int* b = my_malloc(len * sizeof(int));
    int* c = my_malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) {
        a[i] = i;
        b[i] = i+1;
    }
    for (int i = 0; i < len; i++) {
        c[i] = a[i] * b[i];
    }

    // check c holds correct result for all positions
    for (int i = 0; i < len; i++) {
        if (c[i] != a[i] * b[i]) {
            my_free(a); my_free(b); my_free(c);
            print_red("Failed: sample incorrect\n");
            return;
        }
    }

    // check correct number of pages
    if (phl->numPagesAllocated != 12) {
        my_free(a); my_free(b); my_free(c);
        print_red("Failed: wrong number of pages\n");
        return;
    }

    my_free(a); my_free(b); my_free(c);
    print_green("Passed!\n");
}

void test_address_prefix() {
    PageHeaderList* phl = get_page_header_list();
    int len = 10;
    int* a = my_malloc(len * sizeof(int));
    int* b = my_malloc(len * sizeof(int));

    // check address prefix for allocation 'a'
    void* rawPtr = (void*)a - 8;
    PageHeader* phPtr = *((PageHeader**)rawPtr);
    if (phPtr != phl->head) {
        my_free(a); my_free(b);
        printf("Failed: incorrect header pointer: a\n");
        return;
    }

    // check address prefix for allocation 'b'
    rawPtr = (void*)b - 8;
    phPtr = *((PageHeader**)rawPtr);
    if (phPtr != phl->head->next) {
        my_free(a); my_free(b);
        printf("Failed: incorrect header pointer: b\n");
        return;
    }

    my_free(a); my_free(b);
    print_green("Passed!\n");
}

void test_fragmented_alloc() {
    PageHeaderList* phl = get_page_header_list();
    int onePageSize = (phl->pageSize - ALLOC_PREFIX_SIZE);
    int twoPageSize = (2*phl->pageSize - ALLOC_PREFIX_SIZE);
    int *a = my_malloc(onePageSize);
    int *b = my_malloc(twoPageSize);
    int *c = my_malloc(onePageSize);
    my_free(b);

    // allocate 2 pages in the 2 page gap (pages 2 and 3)
    int *d = my_malloc(twoPageSize);
    if (phl->head->next->isFree || phl->head->next->isFree) {
        print_red("Failed\n");
        return;
    }
    my_free(d);

    // allocate 3 pages (must use pages 5, 6 and 7)
    int threePageSize = (3*phl->pageSize - ALLOC_PREFIX_SIZE);
    int *e = my_malloc(threePageSize);
    if (phl->head->next->next->next->next->isFree || 
        phl->head->next->next->next->next->next->isFree ||
        phl->head->next->next->next->next->next->next->isFree) {

        print_red("Failed\n");
        return;
    }
    print_green("Passed!\n");
}

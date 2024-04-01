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
        print_green("Failed\n");
        return;
    }
    print_green("Passed!\n");
}

void test_simple_alloc() {
    int *ptr = (int *)my_malloc(sizeof(int));
    if (ptr == NULL) {
        print_green("Failed\n");
        return;
    }
    ptr[0] = 1;
    if (ptr[0] != 1) {
        print_green("Failed\n");
        return;
    }
    print_green("Passed!\n");
}

void test_multi_page_alloc() {
    PageHeaderList* phl = get_page_header_list();
    void *ptr = my_malloc(phl->pageSize * 3);

    if (phl->numPagesAllocated != 3) {
        print_red("Failed\n");
        return;    
    }
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
            print_red("Failed: sample incorrect\n");
        }
    }

    // check correct number of pages
    if (phl->numPagesAllocated != 12) {
        print_red("Failed: wrong number of pages\n");
        return;
    }
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
        printf("Failed: incorrect header pointer: a\n");
        return;
    }

    // check address prefix for allocation 'b'
    rawPtr = (void*)b - 8;
    phPtr = *((PageHeader**)rawPtr);
    if (phPtr != phl->head->next) {
        printf("Failed: incorrect header pointer: b\n");
        return;
    }

    print_green("Passed!\n");
}

void test_free() {
    PageHeaderList* phl = get_page_header_list();
    int len = (phl->pageSize * 2) - ALLOC_PREFIX_SIZE;
    void *a = my_malloc(len);
    display_page_header_list(10);
    printf("---------------------------\n");
    my_free(a);
    display_page_header_list(10);
}

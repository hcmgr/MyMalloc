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


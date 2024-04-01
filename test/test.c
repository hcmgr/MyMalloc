/**
 * All tests written in this file
*/

#include <stdlib.h>
#include <stdio.h>
#include "testHelpers.h"
#include "../mall.h"

void test_allocation() {
    int *ptr = (int *)my_malloc(sizeof(int));
    if (ptr != NULL) {
        print_green("Passed!\n");
    } else {
        print_red("Failed!\n");
    }
}

void test_zero_allocation() {
    int *ptr = (int *)my_malloc(0);
    if (ptr == NULL) {
        print_green("Passed!\n");
    } else {
        print_red("Failed!\n");
    }
}

void test_distinct_pointers() {
    int *ptr1 = (int *)my_malloc(sizeof(int));
    int *ptr2 = (int *)my_malloc(sizeof(int));
    if (ptr1 != NULL && ptr2 != NULL && ptr1 != ptr2) {
        print_green("Passed!\n");
    } else {
        print_red("Failed!\n");
    }
}
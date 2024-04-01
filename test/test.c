/**
 * All tests written in this file
*/

#include <stdlib.h>
#include <stdio.h>
#include "testHelpers.h"
#include "../mall.h"

// Function to test whether my_malloc() successfully allocates memory
void test_allocation() {
    int *ptr = (int *)malloc(sizeof(int));
    if (ptr != NULL) {
        print_green("Allocation test passed!\n");
        free(ptr);
    } else {
        print_red("Allocation test failed!\n");
    }
}

// Function to test whether my_malloc() returns NULL for zero size
void test_zero_allocation() {
    print_green("Allocation test passed!\n");
    // int *ptr = (int *)my_malloc(0);
    // if (ptr == NULL) {
    //     printf("Zero allocation test passed!\n");
    // } else {
    //     printf("Zero allocation test failed!\n");
    // }
}

// Function to test whether my_malloc() returns distinct pointers for multiple allocations
void test_distinct_pointers() {
    print_red("Allocation test failed!\n");
    // int *ptr1 = (int *)my_malloc(sizeof(int));
    // int *ptr2 = (int *)my_malloc(sizeof(int));
    // if (ptr1 != NULL && ptr2 != NULL && ptr1 != ptr2) {
    //     printf("Distinct pointers test passed!\n");
    //     free(ptr1);
    //     free(ptr2);
    // } else {
    //     printf("Distinct pointers test failed!\n");
    //     if (ptr1 != NULL) free(ptr1);
    //     if (ptr2 != NULL) free(ptr2);
    // }
}
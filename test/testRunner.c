/**
 * Runs all tests from test.c/test.h
*/

#include <stdio.h>
#include "test.h"

typedef struct Test {
    const char* name;
    void (*fn)();
} Test;

Test testList[] = {
    // {"test_zero_alloc", test_zero_alloc},
    // {"test_simple_alloc", test_simple_alloc},
    // {"test_multi_page_alloc", test_multi_page_alloc},
    // {"test_arrays_example", test_arrays_example},
    // {"test_address_prefix", test_address_prefix},
    // {"test_free", test_free},
    {"test_fragmented_alloc", test_fragmented_alloc}
};

/**
 * Run all tests
*/
int runAll() {
    int num_tests = sizeof(testList) / sizeof(testList[0]);

    // Run each test function
    printf("---------------------------\n");
    for (int i = 0; i < num_tests; ++i) {
        printf("%d: %s\n\n\n", i+1, testList[i].name);
        setup();
        testList[i].fn();
        printf("---------------------------\n");
    }
    return 0;
}
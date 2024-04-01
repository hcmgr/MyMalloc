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
    {"test_allocation", test_allocation},
    {"test_zero_allocation", test_zero_allocation},
    {"test_distinct_pointers", test_distinct_pointers}
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
        testList[i].fn();
        printf("---------------------------\n");
    }
    return 0;
}
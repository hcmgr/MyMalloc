#include <stdio.h>

/**
 * 'Passed' test color
*/
void print_green(const char *message) {
    printf("\033[0;32m%s\033[0m", message);
}

/**
 * 'Failed' test color
*/
void print_red(const char *message) {
    printf("\033[0;31m%s\033[0m", message);
}
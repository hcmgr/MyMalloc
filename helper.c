#include <unistd.h>
#include <stdio.h>

#include "helper.h"

/**
 * Returns system page size
*/
int sys_page_size() {
    int page_size = getpagesize();

    if (page_size == -1) {
        perror("Failed to get page size");
        return 0;
    }

    return page_size;
}

/**
 * Peforms integer ceiling operation
*/
int ceil_int(int num, int divisor) {
    return (num + divisor - 1) / divisor;
}
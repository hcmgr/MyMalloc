#include <unistd.h>
#include <stdio.h>

#include "helper.h"

/**
 * Returns system page size
*/
int get_system_page_size() {
    int page_size = getpagesize();

    if (page_size == -1) {
        perror("Failed to get page size");
        return 1;
    }

    return page_size;
}

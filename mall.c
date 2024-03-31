#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
- fully heap-managed malloc()/free() implementation, using sbrk() syscall
    - 


*/

struct HeapHeader {
    int 
}

int main() {
    void *ptr = sbrk(4096); // Extend the heap by 4096 bytes (1 page)
    if (ptr == (void*)-1) {
        perror("sbrk");
        return 1;
    }

    // Use the allocated memory
    *(int*)ptr = 42;

    // Verify that it works
    printf("Value at allocated memory: %d\n", *(int*)ptr);

    return 0;
}

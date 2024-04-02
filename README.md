## MyMalloc

### A from-scratch malloc() / free() implementation

### Implementation Details
##### my_malloc()
- For both simplicity and alignment purposes, allocation is done on a page-basis (as per the output of sysconf(_SC_PAGESIZE))
- New memory is lazily allocated on request, making use of the sbrk() system call to grow the heap in page increments
- Free / allocated pages are kept track of using the PageHeaderList
- The PageHeaderList is a list of PageHeader structures, each of which represents an allocatable page in memory
- The PageHeaderList takes up the first N pages of the heap, where N = 4 by default.
- Sample calculation:
    - assume a page size of 4096 (2**12)
    - 4 pages of 32 byte headers gives you: 2**(12 + 2 - 5) = 512 headers
    - 512 headers -> 512 allocatable pages -> 2**(9 + 12) = 2**21 = 2MB of allocatable memory

##### my_free()
- Upon free'ing, the allocated memory is zero'd and the corresponding PageHeader is updated
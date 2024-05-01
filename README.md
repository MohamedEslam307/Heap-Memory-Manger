# My Heap Memory Manager

This repository provides a custom implementation of a heap memory manager in C. It offers functions for common memory management operations:
    malloc(size): Allocates a block of memory of the specified size on the heap.
    free(ptr): Deallocates a previously allocated memory block pointed to by ptr.
    calloc(nmemb, size): Allocates memory for an array of nmemb elements of size size and initializes all elements to zero.
    realloc(ptr, size): Resizes a previously allocated memory block pointed to by ptr to the new size size.

##Features:
    Efficient Memory Management: Utilizes a doubly linked list to track free memory blocks, enabling efficient allocation and deallocation.
    Reduced Fragmentation: Implements strategies like splitting large free blocks to minimize external fragmentation and improve memory utilization.
    Automatic SBRK Calls: Expands the program break with a large block when necessary to allocate memory for requests that exceed the available free space.

##Building:
    Prerequisites: Ensure you have a C compiler (e.g., GCC) installed on your system.
    Makefile (Optional): navigate to the project directory in your terminal and run:
        make: Builds the library.

##Usage:
    Include the header file (hmm.h) in your source code and use the provided functions like standard C library functions (malloc, free, calloc, realloc). Refer to       the function documentation (man pages or comments within the code) for detailed usage information and parameter descriptions.

##Further Considerations:
    Error Handling: Consider incorporating error handling mechanisms to gracefully handle potential issues during memory allocation and deallocation.
    Testing: Implement some test scripts and other testing strategies to ensure the correctness and robustness of my memory manager code and finally i replaced the glibc allocator with our custom solution. The results were truly impressive, bash run with my heap memory manager 💪🏻🥳

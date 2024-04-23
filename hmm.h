#ifndef HMM_H  // Include guard to prevent multiple inclusions
#define HMM_H

#include <unistd.h>  // For sbrk (if used)
#include "DoubleLinkedList.h"  // Include header for doubly linked list implementation
#include <string.h>

#define METADATA_SIZE 8 // Size of metadata stored with each allocated memory block
#define SBRK_ALLOC_SIZE (33*4096) // Size of memory to request from OS using sbrk
#define MIN_FREE_SBRK (15*4096) // Minimum size of free memory to release using sbrk

void *realloc(void *ptr, size_t size);
void *calloc(size_t nmemb, size_t size);
/**
 * @brief Allocates memory on the heap
 *
 * This function allocates memory on the heap of size 'size' bytes.
 * It first checks the free list for a suitable block. If a suitable block is found,
 * it is removed from the list and returned to the user. Otherwise, it expands the heap
 * using sbrk and splits a newly allocated block to satisfy the request.
 *
 * @param size The size of memory to allocate in bytes
 *
 * @return A pointer to the allocated memory, or NULL on failure
 */
void *malloc(size_t size);

/**
 * @brief Frees memory that was previously allocated by my_malloc
 *
 * This function deallocates memory pointed to by the ptr argument.
 * It finds the corresponding node in the free list and updates the free list accordingly.
 * It also attempts to merge adjacent free blocks if possible.
 * 
 * @note Don't free memroy that was't allocated before and don't free any memory twice
 *
 * @param ptr A pointer to the memory to be freed
 * 
 * @return Nothing
 */
void free(void *ptr);

#endif  // HMM_H

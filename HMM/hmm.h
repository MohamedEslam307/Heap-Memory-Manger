#ifndef HMM_H  // Include guard to prevent multiple inclusions
#define HMM_H

#include <unistd.h>  // For sbrk (if used)
#include "./../DoubleLinkedList/DoubleLinkedList.h"  // Include header for doubly linked list implementation
#include <string.h>

#define METADATA_SIZE 8 // Size of metadata stored with each allocated memory block
#define SBRK_ALLOC_SIZE (4*1024*1024) // Size of memory to request from OS using sbrk
#define MIN_FREE_SBRK (3*1024*1024) // Minimum size of free memory to release using sbrk


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

/**
 * Resizes a previously allocated memory block.
 *
 * This function attempts to resize the memory block pointed to by `ptr`. The new size is specified by `size` bytes.
 * It offers a way to modify the amount of memory allocated for a given block without having to manually free and reallocate.
 *
 * There are three main cases handled by `realloc`:
 *  - If `ptr` is NULL, it behaves exactly like `malloc(size)`.
 *  - If `size` is zero, it frees the memory pointed to by `ptr` and returns NULL.
 *  - If `size` is larger than the current allocation, it allocates a new block of memory, copies the data from the old block,
 *    and frees the old block. Otherwise, it returns the original pointer (`ptr`).
 *
 * @param ptr    A pointer to the previously allocated memory block, or NULL if requesting a new allocation.
 * @param size   The new size of the memory block in bytes. If `size` is zero, the function frees the memory pointed to by `ptr`.
 *
 * @return  A pointer to the reallocated memory block on success, NULL on failure.
 *          Failure can occur if there is not enough memory available for the requested size, or if `ptr` is invalid.
 */
void *realloc(void *ptr, size_t size);
/**
 * Allocates memory and initializes it to zero.
 *
 * This function allocates a contiguous block of memory of size `nmemb * size` bytes.
 * If successful, it initializes all bytes in the allocated block to zero. This ensures that
 * any uninitialized data is set to a known value, which can help prevent security vulnerabilities
 * and unexpected behavior.
 *
 * @param nmemb  The number of elements to allocate. If `nmemb` is zero, the function returns NULL.
 * @param size   The size of each element in bytes. If `size` is zero, the function returns NULL.
 *
 * @return  A pointer to the allocated memory block on success, NULL on failure.
 *          Failure can occur if the product of `nmemb` and `size` overflows `size_t`, or if there
 *          is not enough memory available to satisfy the request.
 */
void *calloc(size_t nmemb, size_t size);

#endif  // HMM_H

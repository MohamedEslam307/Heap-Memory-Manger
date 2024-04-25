/*
 * File: hmm.c
 * Description: my heap memory manger functions: malloc , free , calloc and realloc.
 * Author: Mohamed Eslam
 */

#include "hmm.h" // Include header file for custom data structures and functions
//#include "DoubleLinkedList.c"
//#include "main.c"

node_t *headFreeListNode = NULL; // Global pointer to the head of the free memory list
static uint32_t* programBreak = 0; // Global pointer to the program break


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
void *malloc(size_t size){
    void *retAdd = NULL; // Pointer to the allocated memory, initialized to NULL
    node_t *ptrFreeNode; // Pointer to a free memory block in the free memory list

    // Adjust size to at least the size of a pointer
    if ((size >= 0) && (size < (sizeof(void *) * 2))) {
        size = sizeof(void *) * 2;
    }

    return_status_t ret = NOK; // Return status for function calls
    uint32_t ptrFreeNodeIndex = 0; // Index of found node in the free list
    uint8_t sizeListStat = 0; // Status of size comparison with free memory blocks
    node_t *allocNode = NULL; // Pointer to the allocated memory block
    size = size + METADATA_SIZE; // Add metadata size to requested size
    size = ((size + 7) / 8) * 8; //for allignment
    uint32_t sbrkNum = 0; // Counter for the number of sbrk calls

    // Traverse free list to find free node
    node_t *freeListNode = findNodeSize(headFreeListNode, size, &sizeListStat, &ptrFreeNodeIndex);

    switch (sizeListStat) {
        case EQUIV_REQ:
            retAdd = (uint8_t *)freeListNode + METADATA_SIZE; // Return address after metadata
            ret = removeNode(&headFreeListNode, &freeListNode); // Remove node from free list
            break;
        case LARGER_THAN_REQ:
            /* To reduce external fragmentation */
            if (freeListNode->size >= (size + 24)) {
                allocNode = splitNode(size, freeListNode, &headFreeListNode); // Split free node
            } else {
                allocNode = freeListNode;
                ret = removeNode(&headFreeListNode, &freeListNode); // Remove node from free list
            }
            retAdd = (uint8_t *)allocNode + METADATA_SIZE; // Return address after metadata
            break;
        case SMALLER_THAN_REQ:
            ptrFreeNode = (node_t *)sbrk(SBRK_ALLOC_SIZE);
            sbrkNum++;
            // Check if the size of first allocated node is larger than the program break offset value
            // If smaller, move the program break until there is enough area
            while (size > sbrkNum * SBRK_ALLOC_SIZE) {
                sbrk(SBRK_ALLOC_SIZE);
                sbrkNum++;
            }
            programBreak = (uint32_t *)((uint8_t *)ptrFreeNode + (sbrkNum * SBRK_ALLOC_SIZE));
            ptrFreeNode->size = sbrkNum * SBRK_ALLOC_SIZE;
            if (ptrFreeNode->size >= (size + 24)) {
                allocNode = splitNode(size, ptrFreeNode, &headFreeListNode); // Split free node
                appendNode(freeListNode, ptrFreeNode); //add the free part node in the end of the free list
            } else {
                allocNode = ptrFreeNode;
            }
            retAdd = (uint8_t *)allocNode + METADATA_SIZE;
            break;
        case NULL_PTR:
            // No free list, allocate memory using sbrk
            ptrFreeNode = (node_t *)sbrk(SBRK_ALLOC_SIZE);
            sbrkNum++;

            // If smaller, move the program break until there is enough area
            while (size > sbrkNum * SBRK_ALLOC_SIZE) {
                sbrk(SBRK_ALLOC_SIZE);
                sbrkNum++;
            }

            programBreak = (uint32_t *)((uint8_t *)ptrFreeNode + (sbrkNum * SBRK_ALLOC_SIZE));
            ptrFreeNode->size = sbrkNum * SBRK_ALLOC_SIZE;
            // To reduce external fragmentation
            if (ptrFreeNode->size > (size + 24)) {
                allocNode = splitNode(size, ptrFreeNode, &headFreeListNode);
                headFreeListNode->next = NULL;
                headFreeListNode->prev = NULL;
            } else {
                allocNode = ptrFreeNode;
                headFreeListNode = NULL;
            }
            retAdd = (uint8_t *)allocNode + METADATA_SIZE; // Address returned to the user after the metadata

            break;
        default:
            break;
    }

    return retAdd; // Return pointer to allocated memory
}
/**
 * @brief Frees memory that was previously allocated by my_malloc
 *
 * This function deallocates memory pointed to by the ptr argument.
 * It finds the corresponding node in the free list and updates the free list accordingly.
 * It also attempts to merge adjacent free blocks if possible.
 * 
 * @note Don't free memory that wasn't allocated before and don't free any memory twice
 *
 * @param ptr A pointer to the memory to be freed
 * 
 * @return Nothing
 */
void free(void *ptr) {
    node_t *ptrFreeNode = (node_t *)(ptr - METADATA_SIZE); // Calculate pointer to metadata of memory block to free
    uint8_t appendFlag = 1; // Flag to indicate whether the node should be appended to the free list
    return_status_t ret = NOK; // Return status for function calls
    node_t *tempPtrNode = headFreeListNode; // Temporary pointer to traverse the free list

    if (ptr == NULL) {
        /* Nothing to free if pointer is NULL */
    } else {
        // Check if the free list isn't allocated
        if (NULL == headFreeListNode) {
            ptrFreeNode->prev = NULL;
            ptrFreeNode->next = NULL;
            headFreeListNode = ptrFreeNode; // Set the head of the free list to the freed node
            ret = OK;
        } else {
            uint8_t *nextNodePtr = NULL; 
            size_t counter = 0;
            
            do {
                nextNodePtr = (uint8_t *)tempPtrNode + tempPtrNode->size;
                
                if (nextNodePtr == (uint8_t *)ptrFreeNode) {
                    // Found a free node adjacent to the one being freed, merge them
                    tempPtrNode->size += ptrFreeNode->size;
                    if ((tempPtrNode->prev != NULL) && ((uint8_t *)tempPtrNode->prev + tempPtrNode->prev->size == (uint8_t *)tempPtrNode)) {
                        mergeTwoNodes(tempPtrNode->prev, tempPtrNode); // Merge adjacent free blocks
                    }
                    if ((tempPtrNode->next != NULL) && ((uint8_t *)tempPtrNode + tempPtrNode->size == (uint8_t *)tempPtrNode->next)) {
                        mergeTwoNodes(tempPtrNode, tempPtrNode->next); // Merge adjacent free blocks
                    }
                    appendFlag = 0; // No need to append the freed node, it's merged with another
                    break;
                }
                else if (ptrFreeNode < (node_t *)nextNodePtr) {
                    // Insert the freed node before the current node in the free list
                    addNode(&ptrFreeNode, counter, &headFreeListNode); 
                    if ((tempPtrNode->prev != NULL) && ((uint8_t *)tempPtrNode->prev + tempPtrNode->prev->size == (uint8_t *)tempPtrNode)) {
                        mergeTwoNodes(tempPtrNode->prev, tempPtrNode); // Merge adjacent free blocks
                    }
                    if ((tempPtrNode->next != NULL) && ((uint8_t *)tempPtrNode + tempPtrNode->size == (uint8_t *)tempPtrNode->next)) {
                        mergeTwoNodes(tempPtrNode, tempPtrNode->next); // Merge adjacent free blocks
                    }
                    appendFlag = 0;
                    break;
                } else if ((tempPtrNode->next != NULL) && ((uint8_t *)tempPtrNode + tempPtrNode->size == (uint8_t *)tempPtrNode->next)) {
                    mergeTwoNodes(tempPtrNode, tempPtrNode->next); // Merge adjacent free blocks
                } else {
                    tempPtrNode = tempPtrNode->next;
                }
                counter++;
            } while ((tempPtrNode != NULL));
            // Append the free node to the free list if it wasn't merged with any node
            if (appendFlag) {
                ret = appendNode(headFreeListNode, ptrFreeNode); // Append the free node to the free list
            }
        }
        
        // Check if the last node in the free list can be released
        tempPtrNode = headFreeListNode;
        if (tempPtrNode != NULL) {
            while (tempPtrNode->next != NULL) {
                tempPtrNode = tempPtrNode->next; // Move to the next node in the free list
            }
        }

        if (tempPtrNode->size >= (size_t)MIN_FREE_SBRK) {
            // Check if the last free memory block is adjacent to the program break
            programBreak = sbrk(0);
            if ((uint8_t *)tempPtrNode + tempPtrNode->size == (uint8_t *)programBreak) {
                // Release memory from program break
                programBreak = (uint32_t *)((uint8_t *)programBreak - (tempPtrNode->size));
                if (tempPtrNode->prev == NULL) {
                    headFreeListNode = NULL; // Reset head of the free list if the only node is being released
                } else {
                    (tempPtrNode->prev)->next = NULL; // Disconnect the last node from the free list
                }
                sbrk(-(tempPtrNode->size));
            }
        }
    }
}

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
void *calloc(size_t nmemb, size_t size) {
    if ((nmemb == 0) || (size == 0)) {
        return NULL; // Return NULL if either nmemb or size is zero
    } else {
        unsigned char *ptr = (unsigned char *)malloc(nmemb * size); // Allocate memory
        memset(ptr, 0, (nmemb * size)); // Set memory to zero
        return ptr; // Return pointer to allocated memory
    }
}
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
void *realloc(void *ptr, size_t size) {
    void *newptr = NULL; // Initialize new pointer to NULL

    if (NULL == ptr) {
        newptr = malloc(size); // Allocate memory if ptr is NULL
    } else if (0 == size) {
        free(ptr); // Free memory if size is zero
    } else {

        if (size <= ((node_t *)(ptr - 8))->size - 8) {
            newptr = ptr; // Return ptr if size is smaller or equal than the current allocated size
        } else {
            newptr = malloc(size); // Allocate new memory block
            if(NULL!=newptr){
                memcpy(newptr, ptr, ((node_t *)(ptr - 8))->size - 8); // Copy data to new memory block
                free(ptr); // Free the old memory block
            }
        }
    }
    return newptr; // Return pointer to reallocated memory block
}

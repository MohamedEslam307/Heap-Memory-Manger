/*
 * File: my_malloc.c
 * Description: Implementation of custom memory allocation functions: my_malloc and my_free.
 * Author: Mohamed Eslam
 */

#include "hmm.h" // Include header file for custom data structures and functions


node_t *headFreeListNode = NULL; // Global pointer to the head of the free memory list
static uint32_t* progrmaBreak = 0; // Global pointer to the program break

void *calloc(size_t nmemb, size_t size)
{
    if((nmemb==0)||(size==0)){
        return NULL;
    }else{
        unsigned char *ptr=(unsigned char*)malloc((nmemb*size));
        memset(ptr,0,nmemb*size);
        return ptr;
    }
}
void *realloc(void *ptr, size_t size)
{
    void *newptr=NULL;
    if(NULL==ptr){
        newptr=malloc(size);
    }else if(0==size){
        free(ptr);
    }else{
        newptr=malloc(size);
        if(newptr!=NULL){
            if(size<((node_t *)((uint8_t*)ptr-8))->size)
                memcpy(newptr,ptr,size);
            else
                memcpy(newptr,ptr,((node_t *)(ptr-8))->size);
            free(ptr);
        }
    }
	return newptr;
}
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
    void *retAdd =NULL; // Pointer to the allocated memory, initialized to NULL
    node_t * ptrFreeNode; // Pointer to a free memory block in the free memory list

    // Adjust size to at least the size of a pointer
    if((size>=0)&&(size<(sizeof(void *)*2))){
        size=sizeof(void *)*2;
    }

    return_status_t ret = NOK; // Return status for function calls
    uint32_t ptrFreeNodeIndex=0; // Index of found node in the free list
    uint8_t sizeListStat = 0; // Status of size comparison with free memory blocks
    node_t * allocNode=NULL; // Pointer to the allocated memory block
    size=size+METADATA_SIZE; // Add metadata size to requested size
    size=((size+7)/8)*8;
    uint32_t sbrkNum=0; // Counter for the number of sbrk calls

    // Traverse free list to find free node
    node_t * freeListNode=findNodeSize(headFreeListNode,size,&sizeListStat,&ptrFreeNodeIndex);

    switch (sizeListStat) {
        case EQUIV_REQ:
            retAdd=(uint8_t *)freeListNode+METADATA_SIZE; // Return address after metadata
            ret = removeNode(&headFreeListNode,&freeListNode); // Remove node from free list
            break;
        case LARGER_THAN_REQ:
            if(freeListNode->size >= (size+24)){
                allocNode=splitNode(size,freeListNode,&headFreeListNode); // Split free node
            }else{
                ret = removeNode(&headFreeListNode,&freeListNode); // Remove node from free list
                allocNode = freeListNode;
            }                
            retAdd=(uint8_t *)allocNode+METADATA_SIZE; // Return address after metadata
            break;
        case SMALLER_THAN_REQ:
            ptrFreeNode=(node_t *)sbrk(SBRK_ALLOC_SIZE);
            sbrkNum++;
            // Check if the size of first allocated node is larger than the program break offset value
            // If smaller, move the program break until there is enough area
            while(size>sbrkNum*SBRK_ALLOC_SIZE){
                sbrk(SBRK_ALLOC_SIZE);
                sbrkNum++;
            }
            progrmaBreak=(uint32_t*)((uint8_t*)ptrFreeNode+(sbrkNum*SBRK_ALLOC_SIZE));
            ptrFreeNode->size=sbrkNum*SBRK_ALLOC_SIZE;
            allocNode = splitNode(size,ptrFreeNode,&headFreeListNode);
            retAdd=(uint8_t*)allocNode+METADATA_SIZE;
            appendNode(freeListNode,ptrFreeNode);
            break;
        case NULL_PTR:
            // No free list, allocate memory using sbrk
            ptrFreeNode=(node_t *)sbrk(SBRK_ALLOC_SIZE);
            sbrkNum++;

            // If smaller, move the program break until there is enough area
            while(size>sbrkNum*SBRK_ALLOC_SIZE){
                sbrk(SBRK_ALLOC_SIZE);
                sbrkNum++;
            }

            progrmaBreak=(uint32_t*)((uint8_t*)ptrFreeNode+(sbrkNum*SBRK_ALLOC_SIZE));
            ptrFreeNode->size=sbrkNum*SBRK_ALLOC_SIZE;
            //to reduce external fregmantation
            if(ptrFreeNode->size > (size+24)){
                allocNode = splitNode(size,ptrFreeNode,&headFreeListNode);
                headFreeListNode->next=NULL;
                headFreeListNode->prev=NULL;
            }else{
                allocNode=ptrFreeNode;
                headFreeListNode=NULL;
            }
            retAdd=(uint8_t *)allocNode+METADATA_SIZE; // Address returned to the user after the metadata                
            
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
 * @note Don't free memroy that was't allocated before and don't free any memory twice
 *
 * @param ptr A pointer to the memory to be freed
 * 
 * @return Nothing
 */
void free(void *ptr){
    node_t * ptrFreeNode=(node_t *)(ptr-METADATA_SIZE); // Calculate pointer to metadata of memory block to free
    uint8_t appendFlag=1; // Flag to indicate whether the node should be appended to the free list
    return_status_t ret = NOK; // Return status for function calls
    node_t* tempPtrNode=headFreeListNode; // Temporary pointer to traverse the free list
    if(ptr==NULL){
        /*Nothing*/
    }else{
    // Check if the free list isn't allocated
        if (NULL==headFreeListNode){
            ptrFreeNode->prev = NULL;
            ptrFreeNode->next = NULL;
            headFreeListNode = ptrFreeNode;
            ret = OK;
        }else{
            uint8_t *nextNodePtr=NULL; 
            uint32_t counter=0;
            do{
                nextNodePtr=(uint8_t*)tempPtrNode+tempPtrNode->size;
                if(nextNodePtr==(uint8_t *)ptrFreeNode){
                    tempPtrNode->size+=ptrFreeNode->size;
                    //mergeTwoNodes(tempPtrNode,tempPtrNode->next);
                    if((tempPtrNode->prev!=NULL)&&((uint8_t*)tempPtrNode->prev+tempPtrNode->prev->size==(uint8_t*)tempPtrNode)){
                        mergeTwoNodes(tempPtrNode->prev,tempPtrNode);
                    }
                    if((tempPtrNode->next!=NULL)&&((uint8_t*)tempPtrNode+tempPtrNode->size==(uint8_t*)tempPtrNode->next)){
                        mergeTwoNodes(tempPtrNode,tempPtrNode->next);
                    }
                    appendFlag=0;
                    break;
                }else if(ptrFreeNode<(node_t*)nextNodePtr){
                    addNode(&ptrFreeNode,counter,&headFreeListNode);
                    appendFlag=0;
                    break;
                }else{
                    tempPtrNode=tempPtrNode->next;
                }
                counter++;
            }while((tempPtrNode!=NULL)&&appendFlag);
            // Append the free node to the free list if it was't merged with any node
            if(appendFlag){
                ret = appendNode(headFreeListNode,ptrFreeNode); 
            }
        }
        
        // Check if the last node in the free list can be released
        tempPtrNode=headFreeListNode;
        if(tempPtrNode!=NULL){
            while(tempPtrNode->next!=NULL){
                tempPtrNode=tempPtrNode->next; // Move to the next node in the free list
            }
        }

        if(tempPtrNode->size >= (size_t )MIN_FREE_SBRK){
            // Check if the last free memory block is adjacent to the program break
            //progrmaBreak=sbrk(0);
            if((uint8_t*)tempPtrNode+tempPtrNode->size==(uint8_t*)progrmaBreak){
                // Release memory from program break
                progrmaBreak=(uint32_t *)((uint8_t *)progrmaBreak-(tempPtrNode->size));
                if(tempPtrNode->prev==NULL){
                    headFreeListNode=NULL; // Reset head of the free list if the only node is being released
                }else{
                    (tempPtrNode->prev)->next=NULL; // Disconnect the last node from the free list
                }
                sbrk(-(tempPtrNode->size));
            }
        }
    }
}

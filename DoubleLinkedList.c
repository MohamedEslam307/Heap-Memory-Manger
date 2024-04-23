#include "DoubleLinkedList.h" // Including the header file for the doubly linked list.
return_status_t addNode(node_t **ptrNode,size_t nodeIndex,node_t **ptrHead){
    return_status_t ret = OK;
    node_t *tempNode = NULL;
    uint32_t counter = 0;
    
    // Check if ptrNode is not NULL
    if (NULL != ptrNode) {
        uint32_t listLength = getLength(*ptrHead);
        if (0 == nodeIndex) {
            if((*ptrHead)==NULL){
                (*ptrHead)=*ptrNode;
                (*ptrHead)->next=NULL;
                (*ptrHead)->prev=NULL;
            }else{
                (*ptrNode)->next=(*ptrHead);
                (*ptrNode)->prev=NULL;
                (*ptrHead)->prev=(*ptrNode);
                (*ptrHead)=(*ptrNode);
            }
            //*ptrHead = (*ptrNode)->next; // If the index is 0, remove the first node.
            //removeBeginning(ptrNode);
        } else if (listLength == nodeIndex) {
            appendNode(*ptrHead,*ptrNode); // If the index is the last node, remove the end node.
        } else if (listLength <= nodeIndex) {
            ret = NOK; // If the index is out of range, set the return status to NOK.
        } else {
            node_t *tempNode=*ptrHead;
            while(nodeIndex-1>counter){
                counter++;
                tempNode=tempNode->next;
            }
            (*ptrNode)->prev=tempNode;
            (*ptrNode)->next=tempNode->next;
            tempNode->next->prev=(*ptrNode);
            tempNode->next=(*ptrNode);
            // Update next and prev pointers of adjacent nodes to remove the node.
            //((*ptrNode)->next)->prev = (*ptrNode)->prev;
            //((*ptrNode)->prev)->next = (*ptrNode)->next;
        }
    } else {
        ret = NULLPTR; // If ptrNode is NULL, set the return status to NULLPTR.
    }
    return ret;
}
/**
 * @brief Splits a node in the doubly linked list into two nodes based on a specified size.
 *
 * This function takes the size to be copied (`copySize`), pointers to output allocated and free nodes (`allocNode` and `freeNode`),
 * and a pointer to potentially update the head of the free list (`copyHeadFreeListNode`).
 * It splits the original node into two nodes: one with the requested size and another with the remaining memory.
 * The function updates pointers and assigns the appropriate nodes to the output pointers.
 *
 * @param copySize Size of the data to be copied into the allocated node.
 * @param allocNode Output pointer to receive the allocated node.
 * @param freeNode Output pointer to receive the free node (remaining memory).
 * @param copyHeadFreeListNode Pointer to potentially update the head of the free list (optional).
 * 
 * @return void (no return value).
 */
node_t * splitNode(size_t copySize, node_t *freeNode, node_t **copyHeadFreeListNode) {
    // Check if freeNode is NULL
    node_t *allocNode=NULL;
    if (NULL == freeNode) {
        allocNode = NULL; // If freeNode is NULL, set allocNode to NULL.
    } else {
        size_t tempOldNodeSize=freeNode->size;
        allocNode=(node_t *)((uint8_t *)freeNode+(tempOldNodeSize-copySize));
        freeNode->size=tempOldNodeSize-copySize;
        allocNode->size=copySize;
        if(NULL==(*copyHeadFreeListNode)){
            (*copyHeadFreeListNode)=freeNode;
        }
        /*
        size_t tempOldNodeSize=freeNode->size;
        if((freeNode->prev==NULL)&&(freeNode->next==NULL)){
            allocNode=freeNode;
            allocNode->size=copySize;
            freeNode=(node_t *)((uint8_t *)freeNode+copySize);
            freeNode->size=tempOldNodeSize-copySize;
            freeNode->next=NULL;
            freeNode->prev=NULL;
            *copyHeadFreeListNode=freeNode;
        }else if(freeNode->prev==NULL){
            allocNode=freeNode;
            allocNode->size=copySize;
            freeNode=(node_t *)((uint8_t *)freeNode+copySize);
            freeNode->size=tempOldNodeSize-copySize;
            freeNode->prev=NULL;
            freeNode->next=allocNode->next;
            allocNode->next->prev=freeNode;
            *copyHeadFreeListNode=freeNode;
        }else if(freeNode->next==NULL){
            allocNode=freeNode;
            allocNode->size=copySize;
            freeNode=(node_t *)((uint8_t *)freeNode+copySize);
            freeNode->size=tempOldNodeSize-copySize;
            freeNode->next=NULL;
            freeNode->prev=allocNode->prev;
            allocNode->prev->next=freeNode;
        }
        else{
            allocNode=freeNode;
            allocNode->size=copySize;
            freeNode=(node_t *)((uint8_t *)freeNode+copySize);
            freeNode->size=tempOldNodeSize-copySize;
            freeNode->prev=allocNode->prev;
            freeNode->next=allocNode->next;
            allocNode->prev->next=freeNode;
            allocNode->next->prev=freeNode;
            
        }*/
        /*
        node_t *oldNode = (*freeNode); // Store the freeNode in oldNode.
        size_t tempOldNodeSize = (oldNode)->size; // Store the size of the oldNode.
        allocNode = (node_t *)oldNode; // Allocate memory for the allocNode.
        (*freeNode) = (node_t *)((uint8_t *)oldNode + copySize); // Update freeNode to point to the remaining memory.
        (*freeNode)->size = oldNode->size - copySize; // Update the size of the freeNode.
        allocNode->size = copySize; // Update the size of the allocated node.

        // Update next and prev pointers for the new nodes
        (*freeNode)->next = oldNode->next;
        (*freeNode)->prev = oldNode->prev;

        // Update next and prev pointers of adjacent nodes
        if (oldNode->next != NULL) {
            oldNode->next->prev = (*freeNode);
        } else {
            (*freeNode)->next = NULL;
        }
        if (oldNode->prev != NULL) {
            oldNode->prev->next = (*freeNode);
        } else {
            (*freeNode)->prev = NULL;
            (*copyHeadFreeListNode) = (*freeNode);
        }*/
    }
    return allocNode;
}

/**
 * @brief Removes a node from the doubly linked list at a specified index.
 *
 * This function takes pointers to the head of the list (`ptrHead`), a pointer to store the removed node (`ptrNode`), 
 * and the index of the node to be removed (`index`).
 * It updates the pointers of surrounding nodes to maintain the list structure after removal.
 *
 * @param ptrHead Pointer to the head node of the doubly linked list (may be updated).
 * @param ptrNode Output pointer to receive the removed node.
 * @param index Index of the node to be removed (0-based).
 * 
 * @return return_status_t indicating success (OK) or error (NOK, NULLPTR).
 */
return_status_t removeNode(node_t **ptrHead, node_t **ptrNode) {
    return_status_t ret = OK;
    node_t *tempNode = NULL;
    uint32_t counter = 0;
    
    // Check if ptrNode is not NULL
    if (NULL != ptrNode) {
        uint32_t listLength = getLength(*ptrHead);
        if (((*ptrNode)->prev==NULL) && listLength == 0) {
            *ptrHead = NULL; // If the list is empty and index is 0, set the head to NULL.
        } else if (((*ptrNode)->prev==NULL)) {
            *ptrHead = (*ptrNode)->next; // If the index is 0, remove the first node.
            removeBeginning(ptrNode);
        } else if (((*ptrNode)->next==NULL)) {
            removeEnd(ptrNode); // If the index is the last node, remove the end node.
        } else {
            // Update next and prev pointers of adjacent nodes to remove the node.
            ((*ptrNode)->next)->prev = (*ptrNode)->prev;
            ((*ptrNode)->prev)->next = (*ptrNode)->next;
        }
    } else {
        ret = NULLPTR; // If ptrNode is NULL, set the return status to NULLPTR.
    }
    return ret;
}

/**
 * @brief Removes the last node from the doubly linked list.
 *
 * This function takes a pointer to the head of the list (`ptrHead`).
 * It iterates through the list to find the last node and updates pointers to remove it from the list.
 *
 * @param ptrHead Pointer to the head node of the doubly linked list
 * 
 * @return return_status_t indicating success (OK) or error (NOK, NULLPTR).
 */
return_status_t removeEnd(node_t **ptrNode) {
    return_status_t ret;
    node_t *tempNode = NULL;
    uint32_t counter = 0;

    if (NULL != ptrNode) {
        tempNode = *ptrNode;
        tempNode->prev->next = NULL; // Update pointers to remove the last node.
        tempNode->prev = NULL;
        (*ptrNode) = NULL;          
    } else {
        ret = NULLPTR; // If ptrNode is NULL, set the return status to NULLPTR.
    }
    return ret;
}

/**
 * @brief Removes the first node (head) from the doubly linked list.
 *
 * This function takes a pointer to the head of the list (`ptrHead`).
 * It updates the head pointer to point to the next node and adjusts pointers of surrounding nodes if necessary.
 *
 * @param ptrHead Pointer to the head node of the doubly linked list (may be updated).
 * 
 * @return return_status_t indicating success (OK) or error (NOK, NULLPTR).
 */
return_status_t removeBeginning(node_t **ptrHead) {
    return_status_t ret;
    node_t *tempNode = NULL;
    
    if (NULL != ptrHead) {
        tempNode = *ptrHead;
        *ptrHead = tempNode->next; // Update the head pointer.
        (*ptrHead)-> prev = NULL;
    } else {
        ret = NOK; // If ptrHead is NULL, set the return status to NOK.
    }   
    return ret;
}

/**
 * @brief Finds a node in the list that has sufficient size to accommodate a requested size.
 *
 * This function takes the head of the list (`ptrHead`), a desired size (`copySize`), pointers to output status (`status`) and index (`index`),
 * and searches through the list.
 * The `status` pointer (output) can be used to indicate:
 *   - EQUIV_REQ (1): Node size is exactly equal to the requested size.
 *   - LARGER_THAN_REQ (2): Node size is larger than the requested size.
 *   - SMALLER_THAN_REQ (3): Node size is smaller than the requested size.
 * The `index` pointer (output) holds the index of the found node in the list (0-based).
 *
 * @param ptrHead Pointer to the head node of the doubly linked list.
 * @param copySize Size to be accommodated in the found node.
 * @param status Output pointer to receive a status code (EQUIV_REQ, LARGER_THAN_REQ, SMALLER_THAN_REQ).
 * @param index Output pointer to receive the index of the found node (0-based).
 * 
 * @return node_t* Pointer to the found node, or NULL if no suitable node is found.
 */
node_t * findNodeSize(node_t *ptrHead, size_t copySize, uint8_t *status, uint32_t *index) {
    node_t *retAdd = NULL;
    uint8_t statusFlag = 1;
    // Check if pointers are NULL
    if ((NULL == ptrHead) || (status == NULL) || (NULL == index)) {
        retAdd = NULL;
        *status = NULL_PTR; // Set status to NULL_PTR if any of the pointers are NULL.
    } else {
        uint32_t tempIndex = 0;
        node_t *tempNode = ptrHead;
        
        // Iterate through the list to find a suitable node.
        do {
            if (tempNode->size == copySize) {
                *status = EQUIV_REQ; // Set status to EQUIV_REQ if node size is equal to copySize.
                retAdd = tempNode;
                *index = tempIndex;
                statusFlag = 0;
                break;
            } else if (tempNode->size > (copySize+16)) {
                *status = LARGER_THAN_REQ; // Set status to LARGER_THAN_REQ if node size is larger than copySize.
                retAdd = tempNode;
                *index = tempIndex;
                statusFlag = 0;
                break;
            } else {
                // Node size is smaller than copySize, continue searching.
            }
            retAdd = tempNode;
            tempNode = tempNode->next;
            tempIndex++;
        } while (NULL != tempNode);

        if (statusFlag) {
            *status = SMALLER_THAN_REQ; // Set status to SMALLER_THAN_REQ if no suitable node is found.
        }
    }
    return retAdd;
}

/**
 * @brief Merges two adjacent nodes in the doubly linked list.
 *
 * This function takes pointers to the first node (`ptrFristNode`) and the second node (`ptrSecNode`) to be merged.
 * It assumes the nodes are adjacent in the list.
 * The function combines the sizes of the nodes and updates the pointers of surrounding nodes to reflect the merge.
 *
 * @param ptrFristNode Pointer to the first node to be merged.
 * @param ptrSecNode Pointer to the second node to be merged.
 * 
 * @return return_status_t indicating success (OK) or error (NOK, NULLPTR).
 */
return_status_t mergeTwoNodes(node_t *ptrFristNode, node_t *ptrSecNode) {
    return_status_t ret = NOK;
    // Check if any of the nodes are NULL
    if (NULL == ptrFristNode || NULL == ptrSecNode) {
        ret = NULLPTR; // If any node is NULL, set the return status to NULLPTR.
    } else {
        // Merge the two nodes
        ptrFristNode->size = ptrFristNode->size + ptrSecNode->size;
        ptrFristNode->next=ptrSecNode->next;
        if(ptrSecNode->next!=NULL){ 
            ptrSecNode->next->prev = ptrFristNode;
        }
        ret = OK;
    }
    return ret;
}

/**
 * @brief (Optional) Prints the contents of the doubly linked list to standard output.
 *
 * This function takes the pointer to the head node (`ptrHead`) of the list.
 * It iterates through the list and prints the size of each node.
 *
 * Note: This function might be used for debugging purposes and may not be necessary for core functionality.
 *
 * @param ptrHead Pointer to the head node of the doubly linked list.
 * 
 * @return return_status_t indicating success (OK) or error (NOK) or (NULLPTR).
 */
return_status_t displayList(node_t *ptrHead) {
    return_status_t ret = NOK;
    // Check if ptrHead is NULL
    if (NULL == ptrHead) {
        printf("Their is know list that's a non pointer\n\n");
        ret = NULLPTR; // If ptrHead is NULL, set the return status to NULLPTR.
    } else {
        // Print the contents of the list
        node_t *tempNode = ptrHead;
        if (tempNode->next == NULL) {
            printf("------------------------------------------------------------------------\n");
            printf("address= %p   next= NULL   prev= NULL   size= %lu\n", tempNode,
                                                                    tempNode->size);
            printf("------------------------------------------------------------------------\n");

        } else {
            printf("------------------------------------------------------------------------\n");
            do {
                printf("address= %p   next= %p   prev= %p   size= %lu\n", tempNode,
                                                                        tempNode->next,
                                                                        tempNode->prev,
                                                                        tempNode->size);
                tempNode = tempNode->next;
            } while (NULL != tempNode);
            printf("------------------------------------------------------------------------\n");
            ret = OK;
        }
    }
    return ret;
}

/**
 * @brief Creates a new doubly linked list with a single node.
 *
 * This function takes a pointer to a node (`ptrHead`) that will be used as the head of the list.
 * It initializes the node's size, next, and prev pointers to appropriate values for a single-node list.
 *
 * @param ptrHead Pointer to the node that will become the head of the list.
 * @param copyHeadFreeListNode Pointer to update the head of the free list.
 * 
 * @return return_status_t indicating success (OK) or error (NOK, NULLPTR).
 */
return_status_t createList(node_t *ptrHead, node_t **copyHeadFreeListNode) {
    return_status_t ret = NOK;
    // Check if ptrHead is not NULL
    if (ptrHead != NULL) {
        ptrHead->prev = NULL;
        ptrHead->next = NULL;
        *copyHeadFreeListNode = ptrHead; // Update the head of the free list.
        ret = OK;
    } else {
        ret = NULLPTR; // If ptrHead is NULL, set the return status to NULLPTR.
    } 
    return ret;
}

/**
 * @brief Appends a node to the end of the doubly linked list.
 *
 * This function takes a pointer to the head of the list (`ptrHead`) and a pointer to the node to be appended (`ptrNode`).
 * It updates the pointers of the last node and the new node to link them in the list.
 *
 * @param ptrHead Pointer to the head node of the doubly linked list.
 * @param ptrNode Pointer to the node to be appended to the list.
 * 
 * @return return_status_t indicating success (OK) or error (NOK, NULLPTR).
 */
return_status_t appendNode(node_t *ptrHead, node_t *ptrNode) {
    return_status_t ret = OK;
    // Check if ptrNode and ptrHead are not NULL
    if (NULL != ptrNode || NULL != ptrHead) {
        node_t *tempNode = ptrHead;
        while (NULL != tempNode->next) {
            tempNode = tempNode->next;
        }
        ptrNode->prev = tempNode;
        tempNode->next = ptrNode;
        ptrNode->next = NULL;
    } else {
        ret = NULLPTR; // If ptrNode or ptrHead is NULL, set the return status to NULLPTR.
    }
    return ret;
}

/**
 * @brief Calculates the length (number of nodes) of the doubly linked list.
 *
 * This function takes the pointer to the head node (`ptrHead`) of the list.
 * It iterates through the list and increments a counter for each node encountered.
 *
 * @param ptrHead Pointer to the head node of the doubly linked list.
 * 
 * @return uint32_t The number of nodes in the list.
 */
uint32_t getLength(node_t *ptrHead) {
    node_t *tempNode = NULL;
    uint32_t len = 0;

    // Check if ptrHead is not NULL
    if (NULL != ptrHead) {
        tempNode = ptrHead;
        
        // Iterate through the list and count nodes
        while (tempNode->next != NULL) {
            tempNode = tempNode->next;
            len++;
        }
    } else {
        len = -1; // If ptrHead is NULL, set the length to 0.
    }
    return len;
}

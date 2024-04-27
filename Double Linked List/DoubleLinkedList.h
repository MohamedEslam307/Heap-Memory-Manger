#ifndef _DOUBLE_LINKED_LIST_H  // Include guard to prevent multiple inclusions
#define _DOUBLE_LINKED_LIST_H

#include <stdint.h>  // For standard integer types (size_t)
#include <stdio.h>   // For potential use with displayList (optional)

#define NULL_PTR 0                 // Define NULL pointer macro
#define EQUIV_REQ 1                // Define constant for comparison result (equal)
#define LARGER_THAN_REQ 2          // Define constant for comparison result (larger)
#define SMALLER_THAN_REQ 3         // Define constant for comparison result (smaller)

// Define size_t type if not already defined elsewhere
typedef unsigned long size_t;

// Define node structure for doubly linked list
typedef struct node {
  size_t size;             // Size of the data block in the node
  struct node *next;       // Pointer to the next node in the list
  struct node *prev;       // Pointer to the previous node in the list
} node_t;

// Define enumeration for return status codes
typedef enum {
  OK,                       // Operation successful
  NOK,                       // Operation failed (generic error)
  NULLPTR,                    // Null pointer encountered
} return_status_t;
/**
 * @brief Inserts a node into the doubly linked list at a specified index.
 *
 * This function takes pointers to the head of the list (`ptrHead`), a pointer to the
 * node to be inserted (`ptrNode`), and the index at which to insert (`nodeIndex`). It
 * updates the pointers of surrounding nodes to maintain the list structure after insertion.
 *
 * @param ptrNode Pointer to the node to be inserted into the list.
 * @param nodeIndex Index at which to insert the node (0-based).
 * @param ptrHead Pointer to the head node of the doubly linked list (may be updated).
 *
 * @return return_status_t indicating success (OK), error (NOK, out-of-range index),
 *         or NULL pointer error (NULLPTR).
 */
return_status_t addNode(node_t **ptrNode,size_t nodeIndex,node_t **ptrHead);

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
return_status_t appendNode(node_t *ptrHead,node_t *ptrNode);
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
return_status_t mergeTwoNodes(node_t *ptrFristNode,node_t *ptrSecNode);
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
return_status_t displayList(node_t *ptrHead);
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
return_status_t removeNode(node_t **ptrHead, node_t **ptrNode);
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
return_status_t removeBeginning(node_t **ptrHead);
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
return_status_t removeEnd(node_t **ptrNode);
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
size_t getLength(node_t *ptrHead);
return_status_t addInHead(node_t *ptrNode,size_t nodeIndex,node_t **copyHeadFreeListNode);
#endif
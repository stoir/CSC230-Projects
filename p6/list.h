/**
 * @file list.h
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief list file to create nodes, add nodes
 * pop nodes
 */

#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>

/**
 * enum for type of node
 */
typedef enum {
    NODE_INT,
    NODE_STR,
    NODE_REAL,
} NODE_TYPE;

/**
 * node struct
 */
typedef struct {
    size_t element_size;
    NODE_TYPE type;
    size_t type_size;
} NodeType;

/** Node containing an arbitrary value. */
typedef struct NodeStruct {
    /** Pointer to a function that prints the value in this node.
        @param node Pointer to the node containing the value to print. */
    void (*print)(struct NodeStruct* node);

    /** Pointer to a function that frees memory for this node and the
        value it contains.
        @param node Pointer to the node containing the value to print. */
    void (*destroy)(struct NodeStruct* node);

    /** Pointer to the next node on the list. */
    struct NodeStruct* next;
    struct NodeStruct* back;

    /** Pointer to the value contained in this node. */
    void* val;

    NodeType node_type;
} Node;

/** Representation of a linked list of generic values. */
typedef struct {
    /** Pointer to the first node on the list. */
    Node* head;
    Node* tail;

} GenericList;

typedef void (*node_func)(Node* node, GenericList* list);

/**
 * Make an empty, generic list.
 * @return pointer to a new generic list.
*/
GenericList*
makeList(void);

/**
 * Push the given node onto the front of a list.
 * @param list List to add node node to.
 * @param node Node to push onto the front of list.
*/
void
push(GenericList* list, Node* node);

/**
 * @brief Add the given node to the end of a list.
 * @param list List to add node node to.
 * @param node Node to add onto the end of list.
*/
void
enqueue(GenericList* list, Node* node);

/**
 * @brief Remote the node at the front of a list and return it.
 * @param list List to remove a node from.
 * @return Node remoed from the list, or NULL if the list is empty.
*/
Node*
pop(GenericList* list);

/**
 * @brief remove element at end of list
 * @param list to remove from
 * @return removed node or null
 */
Node*
pop_back(GenericList* list);

/**
 * @brief Free all the memory used to store a list, including memory in its nodes.
 * @param list The list that should be freed.
*/
void
freeList(GenericList* list);

/**
 * @brief print list
 * @param list to print
 */
void
list_print(GenericList* list);

/**
 * @brief destroy
 * @param list to destroy
 */
void
list_destroy(GenericList* list);

#endif

/**
 * @file list.c
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief list file to create nodes, add nodes
 * pop nodes
 */
#include "list.h"

#include <stdlib.h>

/**
 * Make an empty, generic list.
 * @return pointer to a new generic list.
*/
GenericList*
makeList()
{
    GenericList* list = malloc(sizeof(GenericList));
    list->head = NULL;
    return list;
}

/**
 * Push the given node onto the front of a list.
 * @param list List to add node node to.
 * @param node Node to push onto the front of list.
*/
void
push(GenericList* list, Node* node)
{
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->head->back = node;
        node->next = list->head;

        list->head = node;
    }
}

/**
 * @brief Add the given node to the end of a list.
 * @param list List to add node node to.
 * @param node Node to add onto the end of list.
*/
void
enqueue(GenericList* list, Node* node)
{
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        Node* old_tail = list->tail;

        node->back = old_tail;
        old_tail->next = node;

        list->tail = node;
    }
}

/**
 * @brief Remote the node at the front of a list and return it.
 * @param list List to remove a node from.
 * @return Node remoed from the list, or NULL if the list is empty.
*/
Node*
pop(GenericList* list)
{
    if (list->head != NULL) {
        Node* old_head = list->head;
        Node* new_head = list->head->next;

        if (new_head != NULL) {
            new_head->back = NULL;
        } else {
            list->tail = NULL;
        }

        list->head = new_head;

        old_head->next = NULL;
        old_head->back = NULL;
        return old_head;
    } else {
        return NULL;
    }
}

/**
 * @brief remove element at end of list
 * @param list to remove from
 * @return removed node or null
 */
Node*
pop_back(GenericList* list)
{
    if (list->tail != NULL) {
        Node* old_tail = list->tail;
        Node* new_tail = old_tail->back;

        if (new_tail != NULL) {
            new_tail->next = NULL;
        } else {
            list->head = NULL;
        }

        list->tail = new_tail;

        old_tail->next = NULL;
        old_tail->back = NULL;
        return old_tail;
    } else {
        return NULL;
    }
}

/**
 * @brief list each node
 * @param list of nodes
 * @param func node_func
 */
void
list_for_each(GenericList* list, node_func func)
{
    Node* node = list->head;

    while (node != NULL) {
        func(node, list);
        node = node->next;
    }
}

/**
 * @brief print each node
 * @param list of nodes
 * @param node to print
 */
void
print_func(Node* node, GenericList* list)
{
    node->print(node);
}

/**
 * @brief destor ynode
 * @param list of nodes
 * @param node to destroy
 */
void
destory_func(Node* node, GenericList* list)
{
    node->destroy(node);
}

/**
 * @brief print list
 * @param list to print
 */
void
list_print(GenericList* list)
{
    list_for_each(list, print_func);
}

/**
 * @brief destroy
 * @param list to destroy
 */
void
list_destroy(GenericList* list)
{
    list_for_each(list, destory_func);
    list->head = NULL;
    list->tail = NULL;

    free(list);
    list = NULL;
}

/**
 * @brief Free all the memory used to store a list, including memory in its nodes.
 * @param list The list that should be freed.
*/
void
freeList(GenericList* list)
{
    list_destroy(list);
}

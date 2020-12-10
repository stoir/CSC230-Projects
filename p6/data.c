/**
 * @file data.c
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief create int nodes, real nodes and string nodes
 */
#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief destroy node
 * @param ndoe to destroy
 */
void
mijn_destroy(Node* node)
{
    if (node != NULL) {
        free(node->val);
        free(node);
        node = NULL;
    }
}

/**
 * @brief print value of node
 * @param node to print
 */
void
mijn_print(Node* node)
{
    if (node != NULL) {
        if (node->node_type.type == NODE_INT) {
            int val = *(int*) node->val;
            printf("%d\n", val);
        } else if (node->node_type.type == NODE_REAL) {
            double val = *(double*) node->val;
            printf("%.3lf\n", val);
        } else if (node->node_type.type == NODE_STR) {
            char* val = (char*) node->val;
            printf("%s\n", val);
        } else {
        }
    }
}

/**
 * @brief Make a node containing an node type
 * of int, real, or string
 * @param val Integer value to store in this node.
 * @return pointer to the new node.
*/
Node*
make_node(const void* val, NodeType node_type)
{
    size_t element_size = node_type.element_size;
    Node* node = malloc(sizeof(Node));

    node->node_type = node_type;
    node->val = malloc(element_size);
    memcpy(node->val, val, element_size);
    node->next = NULL;

    node->destroy = mijn_destroy;
    node->print = mijn_print;

    return node;
}

/**
 * @brief Make a node containing an integer value.
 * @param val Integer value to store in this node.
 * @return pointer to the new node.
*/
Node*
makeIntNode(int val)
{
    NodeType node_type = {.element_size = sizeof(int),
                          .type = NODE_INT,
                          .type_size = sizeof(int)};
    return make_node(&val, node_type);
}

/**
 * @brief Make a node containing a real number value.
 * @param val Double value to store in this node.
 * @return pointer to the new node.
*/
Node*
makeRealNode(double val)
{
    NodeType node_type = {.element_size = sizeof(double),
                          .type = NODE_REAL,
                          .type_size = sizeof(double)};
    return make_node(&val, node_type);
}

/**
 * @param Make a node containing an arbitrary string value.
 * @param str String value to store in this node.
 * @return pointer to the new node.
*/
Node*
makeStringNode(const char* val)
{
    NodeType node_type = {.element_size = strlen(val) + 1,
                          .type = NODE_STR,
                          .type_size = sizeof(char*)};
    return make_node(val, node_type);
}

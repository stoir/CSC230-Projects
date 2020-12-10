/**
 * @file data.h
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief create int nodes, real nodes and string nodes
 */
#ifndef _DATA_H_
#define _DATA_H_

#include "list.h"

/**
 * @brief Make a node containing an integer value.
 * @param val Integer value to store in this node.
 * @return pointer to the new node.
*/
Node*
makeIntNode(int val);

/**
 * @brief Make a node containing a real number value.
 * @param val Double value to store in this node.
 * @return pointer to the new node.
*/
Node*
makeRealNode(double val);

/**
 * @param Make a node containing an arbitrary string value.
 * @param str String value to store in this node.
 * @return pointer to the new node.
*/
Node*
makeStringNode(char const* str);

#endif

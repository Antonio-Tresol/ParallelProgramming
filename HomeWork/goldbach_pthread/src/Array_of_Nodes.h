// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef ARRAY_OF_NODES_H
#define ARRAY_OF_NODES_H
#include <stddef.h>
#include <stdint.h>
#include <Array_of_Sums.h>
/**
 * @brief A node of information of a dynamic array
 * @var number The number of the node
 * @var number of sums of that number
 */
typedef struct {
  int64_t number;
  int64_t number_of_sums;
  array_of_sums_t* array_of_sums;
} node_t;
/**
 * @brief Initializes a node_t structure.
 * 
 * @param node - the node_t structure to initialize.
 * @param number - the number to initialize the node with.
 * @return int - 0 if successful, -1 otherwise.
 */
int node_init(node_t *node, int64_t number);
/**
 * @brief Destroys a node_t structure.
 * 
 * @param node - the node_t structure to destroy.
 */
void node_destroy(node_t *node);

/**
 * @brief  A structure representing an array of nodes.
 * elements - pointer to the array of elements
 * capacity - capacity of the array
 * count - number of elements in the array
 */
typedef struct {
    node_t* elements;
    uint64_t capacity;
    uint64_t count;
} array_of_nodes_t;
/**
 * Initializes an array_of_nodes_t structure.
 * @param array - the array_of_nodes_t structure to initialize.
 * @return 0 if successful, -1 otherwise.
 */
int array_of_nodes_init(array_of_nodes_t* array);
/**
 * Destroys an array_of_nodes_t structure.
 * @param array - the array_of_nodes_t structure to destroy.
 */
void array_of_nodes_destroy(array_of_nodes_t* array);
/**
 * Appends an element to an array_of_nodes_t structure.
 * @param array - the array_of_nodes_t structure to append to.
 * @param element - the element to append.
 * @return 0 if successful, -1 otherwise.
 */
int array_of_nodes_append(array_of_nodes_t* array, int64_t element);
/**
 * @brief add a void node
 * @param array - the array_of_nodes_t structure to increase the capacity of.
 * @return 0 if successful, -1 otherwise.
 */
int array_of_nodes_append_void(array_of_nodes_t* array);
#endif  // ARRAY_OF_NODES_H

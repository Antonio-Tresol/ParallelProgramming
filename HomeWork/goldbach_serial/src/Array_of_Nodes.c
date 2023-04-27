// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <Array_of_Nodes.h>
#include <assert.h>
#include <stdlib.h>
int node_init(node_t *node, int64_t number) {
  assert(node);
  node->number = number;
  node->number_of_sums = 0;
  node->array_of_sums = NULL;
  return EXIT_SUCCESS;
}
void node_destroy(node_t *node) {
  assert(node);
  if ( node->array_of_sums ) {
    array_of_sums_destroy(node->array_of_sums);
    free(node->array_of_sums);
  }
  node->array_of_sums = NULL;
  node->number_of_sums = 0;
  node->number = 0;
}
int array_of_nodes_init(array_of_nodes_t* array) {
    assert(array);
    array->elements = NULL;
    array->capacity = 0;
    array->count = 0;
    return EXIT_SUCCESS;
}
void array_of_nodes_destroy(array_of_nodes_t* array) {
    assert(array);
    if (array->elements) {
        for ( uint64_t i = 0; i < array->count; i++ ) {
        node_destroy(&array->elements[i]);
        }
        free(array->elements);
    }
    array->elements = NULL;
    array->capacity = 0;
    array->count = 0;
}
int array_of_nodes_increase_capacity(array_of_nodes_t* array) {
    assert(array);  // check if array is valid
    int64_t new_capacity = 10 * (array->capacity ? array->capacity : 1);
    // if array->capacity is 0, then new_capacity is 10
    node_t* new_elements = (node_t*) realloc(array->elements, new_capacity
        * sizeof(node_t));
    // realloc() returns a pointer to the newly allocated memory block,
    // but it does not change the value of the pointer, just the memory block
    if ( new_elements ) {
        array->capacity = new_capacity;
        array->elements = new_elements;
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;  // return 0 to indicate success
}
int array_of_nodes_append(array_of_nodes_t* array, int64_t element) {
    assert(array);
    if ( array->count == array->capacity ) {
        if ( array_of_nodes_increase_capacity(array) != EXIT_SUCCESS ) {
            return EXIT_FAILURE;
        }
    }
    node_t* new_element = (node_t*) calloc(1, sizeof(node_t));
    node_init(new_element, element);
    array->elements[array->count++] = *new_element;
    free(new_element);
    return EXIT_SUCCESS;
}


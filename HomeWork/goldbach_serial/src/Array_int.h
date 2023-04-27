// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef ARRAY_INT_H
#define ARRAY_INT_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Array_int is a dynamic array of integers
 */
typedef struct {
  int64_t* elements;  // pointer to the array of elements
  uint64_t capacity;  //  capacity of the array
  uint64_t count;   //    number of elements in the array
} array_int_t;
/**
 * @brief initialize the array
 * @param array - the array_int_t structure to initialize.
 * @return 0 if successful, -1 otherwise.
 */
int array_int_init(array_int_t* array);
/**
 * Destroys an array_int_t structure.
 * @param array - the array_int_t structure to destroy.
 */
void array_int_destroy(array_int_t* array);
/**
 * Appends an element to an array_double_t structure.
 * @param array - the array_int_t structure to append to.
 * @param element - the element to append.
 * @return 0 if successful, -1 otherwise.
 */
int array_int_append(array_int_t* array, int64_t element);
#endif  // ARRAY_INT_H

// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef ARRAY_SUMS_H
#define ARRAY_SUMS_H
#include <stddef.h>
#include <stdint.h>
#include <Array_int.h>
/**
 * @brief An array of sums, each sum is an array of ints
 * 
 */
typedef struct {
  array_int_t* sums;  // pointer to the array of sums
  size_t capacity;  //  capacity of the array
  size_t count;   //    number of elements in the array
} array_of_sums_t;
/**
 * Initializes an array_of_sums_t structure.
 * @param array - the array_of_sums_t structure to initialize.
 * @return 0 if successful, -1 otherwise.
 */
int array_of_sums_init(array_of_sums_t* array);

/**
 * Destroys an array_of_sums_t structure.
 * @param array - the array_of_sums_t structure to destroy.
 */
void array_of_sums_destroy(array_of_sums_t* array);
/**
 * Appends an element to an array_of_sums_t structure.
 * @param array - the array_of_sums_t structure to append to.
 * @param sum - the element to append.
 * @return 0 if successful, -1 otherwise.
 */
int array_of_sums_append(array_of_sums_t* array, array_int_t* sum);
#endif  // ARRAY_SUMS_H

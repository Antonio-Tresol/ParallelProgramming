// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef ARRAY_COR_H
#define ARRAY_COR_H

#include <stddef.h>
#include <stdint.h>
/**
 * @brief  this struct is used to store the coordinates for a block of data
 * @param start: the start of the block
 * @param finish: the end of the block
 */
typedef struct {
  int64_t start;
  int64_t finish;
} coordinates_t;

/**
 * @brief Array_coordinates is a dynamic array of coordinates
 */
typedef struct {
  coordinates_t* elements;  // pointer to the array of elements
  int64_t capacity;  //  capacity of the array
  int64_t count;   //    number of elements in the array
} array_coordinates_t;
/**
 * @brief initialize the array
 * @param array - the array_coordinates_t structure to initialize.
 * @return 0 if successful, -1 otherwise.
 */
int array_coordinates_init(array_coordinates_t* array);
/**
 * Destroys an array_coordinates_t structure.
 * @param array - the array_coordinates_t structure to destroy.
 */
void array_coordinates_destroy(array_coordinates_t* array);
/**
 * Appends an element to an array_coordinates_t structure.
 * @param array - the array_coordinates_t structure to append to.
 * @param coordinates - the element to append.
 * @return 0 if successful, -1 otherwise.
 */
int array_coordinates_append(array_coordinates_t* array,
  coordinates_t coordinates);
#endif  // ARRAY_INT_H

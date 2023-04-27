//  Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef ARRAY_REENTRANT_H
#define ARRAY_REENTRANT_H

#include <stddef.h>

typedef struct array array_reentrant_t;

static const size_t array_not_found = (size_t)-1;
/// @brief create a new array
/// @param capacity the initial capacity of the array
/// @return 
array_reentrant_t* array_create(size_t capacity);
/// @brief destroy the array
/// @param array the array to destroy
void array_destroy(array_reentrant_t* array);
/// @brief get the size of the array
/// @param array the array
/// @param element the size of the element
/// @return the size of the array
int array_append(array_reentrant_t* array, void* element);
/// @brief get the size of the array
size_t array_get_count(const array_reentrant_t* array);
/// @brief get the capacity of the array
/// @brief array the array
void* array_get_element(array_reentrant_t* array, size_t index);
/// @brief finds the index of the element at the beginning of the array
/// @param array the array
/// @param element the element to find
/// @param start_pos the start position
/// @return a size_t value that is the index of the element
size_t array_find_first(const array_reentrant_t* array, const void* element,
  size_t start_pos);
/// @brief  removes the element at the first position
/// @param array 
/// @param element 
/// @param start_pos 
/// @return 
int array_remove_first(array_reentrant_t* array, const void* element, size_t start_pos);

#endif // ARRAY_H

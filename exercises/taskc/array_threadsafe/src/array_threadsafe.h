//  Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef ARRAY_THREADSAFE_H
#define ARRAY_THREADSAFE_H

#include <stddef.h>

typedef struct array array_mutex_t;

static const size_t array_not_found = (size_t)-1;
/// @brief create a new array
/// @param capacity 
/// @return a pointer to the array
array_mutex_t* array_create(size_t capacity);
/// @brief destroy the array
/// @param array the array to destroy
void array_destroy(array_mutex_t* array);
/// @brief get the size of the array
/// @param array the array
size_t array_get_count(array_mutex_t *array);
/// @brief gets an element from the array
/// @param array the array
/// @param index the index of the element
/// @return a pointer to the element
void* array_get_element(array_mutex_t* array, size_t index);
/// @brief appends an element to the array
/// @param array the array
/// @param element element to append
/// @return an int value with the result of the operation
int array_append(array_mutex_t* array, void* element);
/// @brief finds the index of the element at the beginning of the array
/// @param array the array
/// @param element the element to find
/// @param start_pos the start position
/// @return 
size_t array_find_first(array_mutex_t *array, const void* element, size_t start_pos);
/// @brief  removes the element at the first position
/// @param array
/// @param element
int array_remove_first(array_mutex_t* array, const void* element, size_t start_pos);

#endif // ARRAY_H

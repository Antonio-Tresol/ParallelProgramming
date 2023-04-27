//  Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef array_mutex_THREADSAFE_H
#define array_mutex_THREADSAFE_H

#include <stddef.h>

typedef struct array_mutex array_mutex_t;

static const size_t array_mutex_not_found = (size_t)-1;
/// @brief create a new array_mutex
/// @param capacity 
/// @return a pointer to the array_mutex
array_mutex_t* array_mutex_create(size_t capacity);
/// @brief destroy the array_mutex
/// @param array_mutex the array_mutex to destroy
void array_mutex_destroy(array_mutex_t* array_mutex);
/// @brief get the size of the array_mutex
/// @param array_mutex the array_mutex
size_t array_mutex_get_count(array_mutex_t *array_mutex);
/// @brief gets an element from the arry_mutex
/// @param array_mutex the array_mutex
/// @param index the index of the element
/// @return a pointer to the element
void* array_mutex_get_element(array_mutex_t* array_mutex, size_t index);
/// @brief appends an element to the array_mutex
/// @param array_mutex the array_mutex
/// @param element element to append
/// @return an int value with the result of the operation
int array_mutex_append(array_mutex_t* array_mutex, void* element);
/// @brief finds the index of the element at the beginning of the array_mutex
/// @param array_mutex the array_mutex
/// @param element the element to find
/// @param start_pos the start position
/// @return 
size_t array_mutex_find_first(array_mutex_t *array_mutex, const void* element, size_t start_pos);
/// @brief  removes the element at the first position
/// @param array_mutex
/// @param element
int array_mutex_remove_first(array_mutex_t* array_mutex, const void* element, size_t start_pos);

#endif // array_mutex_H

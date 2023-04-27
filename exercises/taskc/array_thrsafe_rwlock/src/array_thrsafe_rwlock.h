//  Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef ARRAY_THRSAFE_RWLOCK_H
#define ARRAY_THRSAFE_RWLOCK_H
#include <pthread.h>
#include <stddef.h>

typedef struct array array_rwlock_t;

static const size_t array_not_found = (size_t)-1;
/// @brief create a new array
/// @param capacity 
/// @return a pointer to the new array
array_rwlock_t* array_create(size_t capacity);
/// @brief destroy an array
/// @param array 
void array_destroy(array_rwlock_t* array);
/// @brief get the size of the array
/// @param array
size_t array_get_count(array_rwlock_t *array);
/// @brief get the capacity of the array
/// @param array is the array
/// @return the capacity of the array
void* array_get_element(array_rwlock_t* array, size_t index);
/// @brief add an element to the array
/// @param array 
/// @param element 
/// @return and error code
int array_append(array_rwlock_t* array, void* element);
/// @brief remove an element from the array
/// @param array the array
/// @param start_pos the position to start the search
size_t array_find_first(array_rwlock_t *array, const void* element, size_t start_pos);
/// @brief remove an element from the array
/// @param array the array
/// @param start_pos the position to start the search
/// @return error code
int array_remove_first(array_rwlock_t* array, const void* element, size_t start_pos);

#endif // ARRAY_THRSAFE_RWLOCK_H

//  Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef array_rwlock_THRSAFE_RWLOCK_H
#define array_rwlock_THRSAFE_RWLOCK_H
#include <pthread.h>
#include <stddef.h>

typedef struct array array_rwlock_t;

static const size_t array_rwlock_not_found = (size_t)-1;
/// @brief create a new array_rwlock
/// @param capacity 
/// @return a pointer to the new array_rwlock
array_rwlock_t* array_rwlock_create(size_t capacity);
/// @brief destroy an array_rwlock
/// @param array_rwlock 
void array_rwlock_destroy(array_rwlock_t* array_rwlock);
/// @brief get the size of the array_rwlock
/// @param array_rwlock
size_t array_rwlock_get_count(array_rwlock_t *array_rwlock);
/// @brief get the capacity of the array_rwlock
/// @param array_rwlock is the array_rwlock
/// @return the capacity of the array_rwlock
void* array_rwlock_get_element(array_rwlock_t* array_rwlock, size_t index);
/// @brief add an element to the array_rwlock
/// @param array_rwlock 
/// @param element 
/// @return and error code
int array_rwlock_append(array_rwlock_t* array_rwlock, void* element);
/// @brief remove an element from the array_rwlock
/// @param array_rwlock the array_rwlock
/// @param start_pos the position to start the search
size_t array_rwlock_find_first(array_rwlock_t *array_rwlock, const void* element, size_t start_pos);
/// @brief remove an element from the array_rwlock
/// @param array_rwlock the array_rwlock
/// @param start_pos the position to start the search
/// @return error code
int array_rwlock_remove_first(array_rwlock_t* array_rwlock, const void* element, size_t start_pos);

#endif // array_rwlock_THRSAFE_RWLOCK_H

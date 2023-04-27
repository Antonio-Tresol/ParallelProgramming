//  Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <assert.h>
#include <stdlib.h>

#include "array_reentrant.h"

/// @brief an array structure
/// @details this structure is used to store the array data
/// @param elements the array of elements
/// @param capacity the capacity of the array
/// @param count the number of elements in the array
typedef struct array {
	void** elements;
	size_t capacity;
	size_t count;
} array_reentrant_t;


array_reentrant_t* array_create(size_t capacity) {
	assert(capacity);
	array_reentrant_t* array = calloc(1, sizeof(array_reentrant_t));
	if ( array == NULL )
		return NULL;
	array->capacity = capacity;
	array->count = 0;
	array->elements = (void**)malloc( capacity * sizeof(void*) );
	if ( array->elements == NULL )
		return free(array), NULL;
	return array;
}

void array_destroy(array_reentrant_t* array) {
	assert(array);
	free(array->elements);
	free(array);
}

int array_increase_capacity(array_reentrant_t* array) {
	assert(array);
	size_t new_capacity = 10 * array->capacity;
	void** new_elements = (void**)realloc( array->elements, new_capacity *
		sizeof(void*) );
	if ( new_elements == NULL )
		return -1;
	array->capacity = new_capacity;
	array->elements = new_elements;
	return 0; // Success
}

int array_decrease_capacity(array_reentrant_t* array) {
	assert(array);
	size_t new_capacity = array->capacity / 10;
	if ( new_capacity < 10 )
		return 0;
	void** new_elements = (void**)realloc( array->elements, new_capacity *
		sizeof(void*) );
	if ( new_elements == NULL )
		return -1;
	array->capacity = new_capacity;
	array->elements = new_elements;
	return 0; // Success
}

size_t array_get_count(const array_reentrant_t* array) {
	assert(array);
	return array->count;
}

void* array_get_element(array_reentrant_t* array, size_t index) {
	assert(array);
	assert( index < array_get_count(array) );
	return array->elements[index];
}

int array_append(array_reentrant_t* array, void* element) {
	assert(array);
	if ( array->count == array->capacity )
		if ( array_increase_capacity(array) )
			return -1;
	assert( array->count < array->capacity );
	array->elements[array->count++] = element;
	return 0; // Success
}
size_t array_find_first(const array_reentrant_t* array, const void* element,
	size_t start_pos) {
	assert( array );
	for ( size_t index = start_pos; index < array->count; ++index )
		if ( array->elements[index] == element )
			return index;
	return array_not_found;
}

int array_remove_first(array_reentrant_t* array, const void* element, size_t start_pos) {
	assert( array );
	size_t index = array_find_first(array, element, start_pos);
	if ( index == array_not_found )
		return -1;
	for ( --array->count; index < array->count; ++index )
		array->elements[index] = array->elements[index + 1];
	if ( array->count == array->capacity / 10 )
		array_decrease_capacity(array);
	return 0; // Removed
}


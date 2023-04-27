//  Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>

#include "array_threadsafe.h"
/// @brief an array_mutex structure
/// @details this structure is used to store the array_mutex data
/// @param elements the array_mutex of elements
/// @param capacity the capacity of the array_mutex
/// @param count the number of elements in the array_mutex
/// @param mutex the mutex to protect the array_mutex
typedef struct array_mutex {
	void** elements;
	size_t capacity;
	size_t count;
	pthread_mutex_t mutex;
} array_mutex_t;

// Private declarations
size_t array_mutex_find_first_private(array_mutex_t* array_mutex, const void* element,
	size_t start_pos);


array_mutex_t* array_mutex_create(size_t capacity) {
	assert(capacity);
	array_mutex_t* array_mutex = calloc(1, sizeof(array_mutex_t));
	if ( array_mutex == NULL )
		return NULL;
	array_mutex->capacity = capacity;
	array_mutex->count = 0;
	pthread_mutex_init( &array_mutex->mutex, NULL );
	array_mutex->elements = (void**)malloc( capacity * sizeof(void*) );
	if ( array_mutex->elements == NULL )
		return (void)free(array_mutex), NULL;
	return array_mutex;
}

void array_mutex_destroy(array_mutex_t* array_mutex) {
	assert(array_mutex);
	pthread_mutex_destroy(&array_mutex->mutex);
	free(array_mutex->elements);
	free(array_mutex);
}

int array_mutex_increase_capacity(array_mutex_t* array_mutex) {
	assert(array_mutex);
	size_t new_capacity = 10 * array_mutex->capacity;
	void** new_elements = (void**)realloc( array_mutex->elements, new_capacity * sizeof(void*) );
	if ( new_elements == NULL )
		return -1;
	array_mutex->capacity = new_capacity;
	array_mutex->elements = new_elements;
	return 0; // Success
}

int array_mutex_decrease_capacity(array_mutex_t* array_mutex) {
	assert(array_mutex);
	size_t new_capacity = array_mutex->capacity / 10;
	if ( new_capacity < 10 )
		return 0;
	void** new_elements = (void**)realloc( array_mutex->elements, new_capacity * sizeof(void*) );
	if ( new_elements == NULL )
		return -1;
	array_mutex->capacity = new_capacity;
	array_mutex->elements = new_elements;
	return 0; // Success
}

size_t array_mutex_get_count(array_mutex_t* array_mutex) {
	assert(array_mutex);
	pthread_mutex_lock( &array_mutex->mutex );
	size_t result = array_mutex->count;
	pthread_mutex_unlock( &array_mutex->mutex );
	return result;
}

void* array_mutex_get_element(array_mutex_t* array_mutex, size_t index) {
	assert(array_mutex);
	assert( index < array_mutex_get_count(array_mutex) );
	pthread_mutex_lock( &array_mutex->mutex );
	void* result = array_mutex->elements[index];
	pthread_mutex_unlock( &array_mutex->mutex );
	return result;
}

int array_mutex_append(array_mutex_t* array_mutex, void* element) {
	assert(array_mutex);
	pthread_mutex_lock( &array_mutex->mutex );
	if ( array_mutex->count == array_mutex->capacity )
		if ( array_mutex_increase_capacity(array_mutex) )
			return (void)pthread_mutex_unlock( &array_mutex->mutex ), -1;
	assert( array_mutex->count < array_mutex->capacity );
	array_mutex->elements[array_mutex->count++] = element;
	pthread_mutex_unlock( &array_mutex->mutex );
	return 0; // Success
}

size_t array_mutex_find_first(array_mutex_t* array_mutex, const void* element, size_t start_pos) {
	assert( array_mutex );
	pthread_mutex_lock( &array_mutex->mutex );
	size_t result = array_mutex_find_first_private(array_mutex, element, start_pos);
	pthread_mutex_unlock( &array_mutex->mutex );
	return result;
}

size_t array_mutex_find_first_private(array_mutex_t* array_mutex, const void* element,
	size_t start_pos) {
	assert( array_mutex );
	for ( size_t index = start_pos; index < array_mutex->count; ++index )
		if ( array_mutex->elements[index] == element )
			return index;
	return array_mutex_not_found;
}

int array_mutex_remove_first(array_mutex_t* array_mutex, const void* element, size_t start_pos) {
	assert( array_mutex );
	pthread_mutex_lock( &array_mutex->mutex );
	size_t index = array_mutex_find_first_private(array_mutex, element, start_pos);
	if ( index == array_mutex_not_found )
		return (void)pthread_mutex_unlock( &array_mutex->mutex ), -1;
	for ( --array_mutex->count; index < array_mutex->count; ++index )
		array_mutex->elements[index] = array_mutex->elements[index + 1];
	if ( array_mutex->count == array_mutex->capacity / 10 )
		array_mutex_decrease_capacity(array_mutex);
	pthread_mutex_unlock( &array_mutex->mutex );
	return 0; // Removed
}

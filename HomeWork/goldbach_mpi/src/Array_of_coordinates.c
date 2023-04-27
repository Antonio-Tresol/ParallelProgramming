// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include "Array_of_coordinates.h"
#include <assert.h>
#include <stdlib.h>

int array_coordinates_init(array_coordinates_t * array) {
    assert(array);
    array->elements = NULL;
    array->capacity = 0;
    array->count = 0;
    return EXIT_SUCCESS;
}
void array_coordinates_destroy(array_coordinates_t* array) {
    assert(array);
    free(array->elements);
    array->elements = NULL;
    array->capacity = 0;
    array->count = 0;
}
int array_coordinates_increase_capacity(array_coordinates_t* array) {
    assert(array);
    int64_t new_capacity = 10 * (array->capacity ? array->capacity : 1);
    coordinates_t* new_elements = (coordinates_t*)
        realloc(array->elements, new_capacity * sizeof(coordinates_t));
    if ( new_elements ) {
        array->capacity = new_capacity;
        array->elements = new_elements;
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
int array_coordinates_append(array_coordinates_t* array,
    coordinates_t coordinates) {
    assert(array);
    if ( array->count == array->capacity ) {
        if ( array_coordinates_increase_capacity(array) != EXIT_SUCCESS ) {
            return EXIT_FAILURE;
        }
    }
    array->elements[array->count++] = coordinates;
    return EXIT_SUCCESS;
}

// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <Array_int.h>
#include <assert.h>
#include <stdlib.h>

int array_int_init(array_int_t * array) {
    assert(array);
    array->elements = NULL;
    array->capacity = 0;
    array->count = 0;
    return EXIT_SUCCESS;
}
void array_int_destroy(array_int_t* array) {
    assert(array);
    free(array->elements);
    array->elements = NULL;
    array->capacity = 0;
    array->count = 0;
}
int array_int_increase_capacity(array_int_t* array) {
    assert(array);
    int64_t new_capacity = 10 * (array->capacity ? array->capacity : 1);
    int64_t* new_elements = (int64_t*) realloc(array->elements, new_capacity
        * sizeof(int64_t));
    if ( new_elements ) {
        array->capacity = new_capacity;
        array->elements = new_elements;
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
int array_int_append(array_int_t* array, int64_t element) {
    assert(array);
    if ( array->count == array->capacity ) {
        if ( array_int_increase_capacity(array) != EXIT_SUCCESS ) {
            return EXIT_FAILURE;
        }
    }
    array->elements[array->count++] = element;
    return EXIT_SUCCESS;
}

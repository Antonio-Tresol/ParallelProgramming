#include "array_double.h"
#include <assert.h>
#include <stdlib.h>


int array_double_init(array_double_t* array) {
    assert(array);
    array->elements = NULL;
    array->capacity = 0;
    array->count = 0;

}

void array_double_destroy(array_double_t* array) {  
    assert(array);
    free(array->elements);
    array->elements = NULL;
    array->capacity = 0;
    array->count = 0;
}

int array_double_increase_capacity(array_double_t* array) {
    assert(array);  // check if array is valid
    size_t new_capacity = 10 * (array->capacity ? array->capacity : 1); // if array->capacity is 0, then new_capacity is 10
    double* new_elements = (double*) realloc(array->elements, new_capacity * sizeof(double)); // realloc() returns a pointer to the newly allocated memory block, 
    //but it does not change the value of the pointer, just the memory block itself.

    if (new_elements) {  // if realloc() returns NULL, the allocation failed.
        array->capacity = new_capacity;  // update the array->capacity to double its current value  
        array->elements = new_elements;  // update the array->elements pointer to point to the newly allocated memory block
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;  // return 0 to indicate success
}


int array_double_append(array_double_t* array, double element) {
    assert(array);
    if (array->count == array->capacity) {
        if(array_double_increase_capacity(array) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }
    array->elements[array->count++] = element;
    return EXIT_SUCCESS;
}

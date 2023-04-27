#ifndef ARRAY_DOUBLE_H
#define ARRAY_DOUBLE_H
#include <stddef.h>


typedef struct {
    double* elements;  // pointer to the array of elements
    size_t capacity;  //  capacity of the array
    size_t count;   //    number of elements in the array
} array_double_t;
/**
 * Initializes an array_double_t structure.
 * @param array - the array_double_t structure to initialize.
 * @return 0 if successful, -1 otherwise.
 */
int array_double_init(array_double_t* array);

/**
 * Destroys an array_double_t structure.
 * @param array - the array_double_t structure to destroy.
 */
void array_double_destroy(array_double_t* array);
/**
 * Appends an element to an array_double_t structure.
 * @param array - the array_double_t structure to append to.
 * @param element - the element to append.
 * @return 0 if successful, -1 otherwise.
 */
int array_double_append(array_double_t* array, double element);




#endif // ARRAY_DOUBLE_H
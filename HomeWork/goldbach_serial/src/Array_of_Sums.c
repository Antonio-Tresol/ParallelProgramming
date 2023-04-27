// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <Array_of_Sums.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
int array_of_sums_init(array_of_sums_t* array) {
  assert(array);
  array->sums = NULL;
  array->capacity = 0;
  array->count = 0;
  return EXIT_SUCCESS;
}
void array_of_sums_destroy(array_of_sums_t* array) {
  for (uint64_t i = 0; i < array->count; i++) {
    array_int_destroy(&array->sums[i]);
  }
  free(array->sums);
  array->sums = NULL;
  array->capacity = 0;
  array->count = 0;
}
int array_of_sums_increase_capacity(array_of_sums_t* array) {
  assert(array);  // check if array is valid
  size_t new_capacity = 10 * (array->capacity ? array->capacity : 1);
  array_int_t* new_sums = (array_int_t*) realloc(array->sums, new_capacity *
  sizeof(array_int_t));
  if ( new_sums ) {  // if realloc() returns NULL, the allocation failed.
    array->capacity = new_capacity;
    array->sums = new_sums;
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;  // return 0 to indicate success
}
int array_of_sums_append(array_of_sums_t* array, array_int_t* sum) {
  assert(array);
  if ( array->count == array->capacity ) {
    if ( array_of_sums_increase_capacity(array) != EXIT_SUCCESS ) {
      return EXIT_FAILURE;
    }
  }
  array->sums[array->count++] = *sum;
  return EXIT_SUCCESS;
}

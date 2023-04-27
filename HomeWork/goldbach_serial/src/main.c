// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include "input_output.h"
#include "Array_of_Nodes.h"
#include "Array_int.h"
#include "Array_of_Sums.h"
#include "inttypes.h"
#include "goldbach.h"
#include "abs_number.h"
int main() {
    array_of_nodes_t* array_of_nodes = (array_of_nodes_t*)
        malloc(sizeof(array_of_nodes_t));
    array_of_nodes_init(array_of_nodes);
    array_int_t* primes = (array_int_t*)
        malloc(sizeof(array_int_t));
    array_int_init(primes);
    uint64_t* ptr_limit;
    // A := read_numbers()
    ptr_limit = read_numbers(array_of_nodes);
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    uint64_t limit = *ptr_limit;
    bool* sieve = (bool*) calloc(limit+1, sizeof(bool));
    // Sieve := sieve_atkin()
    Atkin_sieve(sieve, limit);
    // P := get_array_of_primes(Sieve)
    fill_array_with_primes(sieve, primes, limit);
    uint64_t i;
    // for i:= 1 to A.count do
    for ( i = 0; i < array_of_nodes->count; i++ ) {
        if ( array_of_nodes->elements[i].number == -1 ||
            array_of_nodes->elements[i].number == 0 ) {
            continue;
        } else if (abs_number(array_of_nodes->elements[i].number) <= 5) {
                continue;
        }
        // goldbach_sums(A[i], P)
        goldbach_sums_main(&array_of_nodes->elements[i], primes, sieve, limit);
    }
    // print_goldbach_sums(A)
    print_goldbach_sums(array_of_nodes);
    free(sieve);
    array_of_nodes_destroy(array_of_nodes);
    free(array_of_nodes);
    free(ptr_limit);
    array_int_destroy(primes);
    free(primes);
    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
    printf("Execution time: %.9lfs\n", elapsed_time);
    return 0;
}

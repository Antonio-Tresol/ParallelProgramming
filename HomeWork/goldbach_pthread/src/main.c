// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <inttypes.h>
#include "input_output.h"
#include "Array_of_Nodes.h"
#include "Array_int.h"
#include "Array_of_Sums.h"
#include "goldbach.h"
int main(int argc, char *argv[]) {
    // thread count is the number of processors
    int64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (argc > 1) {
        thread_count = strtoll(argv[1], NULL, 10);
        if (errno == ERANGE) {
            fprintf(stderr, "Error: invalid number of threads");
        }
    }
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
    // set Shared_memory shared_mem:
    // Array_of_Nodes := A
    // Array_of_Primes := P
    // Array_of_Booleans := Sieve
    shared_mem_t* share_mem = (shared_mem_t*)
        malloc(sizeof(shared_mem_t));
    shared_mem_init(share_mem, array_of_nodes, sieve, primes,
        array_of_nodes->count, limit);
    // distribute(shared_mem, int thread_count, mem)
    thread_mem_t* mem = (thread_mem_t*) calloc(thread_count,
        sizeof(thread_mem_t));
    distribute(share_mem, thread_count, mem);
    // for i:= 1 to A.count do
    compute_goldbach(mem, thread_count);
    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
    // print_goldbach_sums(A)
    int64_t total = get_total_number_of_sums(array_of_nodes);
    printf("Total %" PRIu64 " numbers %" PRId64 " sums\n",
      array_of_nodes->count, total);
    print_goldbach_sums(array_of_nodes);
    printf("\nExecution time: %.9lfs\n", elapsed_time);
    shared_mem_destroy(share_mem);
    free(ptr_limit);
    free(mem);
    return 0;
}

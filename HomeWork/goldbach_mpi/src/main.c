// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>
#include "ProcessesCoordination.h"

int main(int argc, char *argv[]) {
  // thread count is the number of processors
  int64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc > 1) {
    thread_count = strtoll(argv[1], NULL, 10);
    if (errno == ERANGE) {
       fprintf(stderr, "Error: invalid number of threads");
    }
  }
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;
    MPI_Comm_rank(world, &process_number);

    int process_count = -1;
    MPI_Comm_size(world, &process_count);
    // printf("process_count %d\n", process_count);
    char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
    int hostname_length = -1;
    MPI_Get_processor_name(process_hostname, &hostname_length);

    array_of_nodes_t* array_of_nodes = (array_of_nodes_t*)
        malloc(sizeof(array_of_nodes_t));
    array_of_nodes_init(array_of_nodes);
    array_int_t* primes = (array_int_t*)
        malloc(sizeof(array_int_t));
    // A := read_numbers()
    int64_t max = 0;

    bool* sieve = NULL;

    shared_mem_t* shared_mem = (shared_mem_t*)
        malloc(sizeof(shared_mem_t));

    if (process_count == 1) {
        compute_goldbach_alone(shared_mem, array_of_nodes, max, primes, sieve,
            thread_count);
    } else {
        compute_goldbach_together(shared_mem, array_of_nodes, max, primes,
            sieve, process_number, process_count, thread_count);
    }
    shared_mem_destroy(shared_mem);
    MPI_Finalize();
    return 0;
  } else {
    fprintf(stderr, "Error: MPI_Init failed");
    return EXIT_FAILURE;
  }
}

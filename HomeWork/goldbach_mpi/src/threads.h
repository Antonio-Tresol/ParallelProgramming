// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef THREADS_H
#define THREADS_H
#include <stddef.h>
#include <assert.h>
#include <stdint.h>
#include "input_output.h"
#include "Array_of_Nodes.h"
#include "Array_int.h"
#include "Array_of_Sums.h"
  /*!< Computes the minimum of \a a and \a b. */
/**
 * @brief share memory between threads
 * @param array_of_nodes nodes of information
 * @param sieve an array of bools mask for a sieve.
 * @param primes an array of ints with primes.
 * @param max the maximum number received
 */
typedef struct {
  array_of_nodes_t *array_of_nodes;
  array_int_t * primes;
  bool* sieve;
  int64_t max;
} shared_mem_t;

/**
 * @brief inits the shared memory
 * @param shared_mem the shared memory to init
 * @param array_of_nodes nodes of information
 * @param sieve an array of bools mask for a sieve.
 * @param primes an array of ints with primes.
 * @param max the maximum number received
 */
void shared_mem_init(shared_mem_t *shared_mem, array_of_nodes_t *array_of_nodes,
  bool* sieve, array_int_t * primes, int64_t max);
/**
 * @brief Destroys the shared memory
 * @param shared_mem the shared memory to destroy
 */
void shared_mem_destroy(shared_mem_t *shared_mem);

#endif

// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef THREADS_H
#define THREADS_H
#include <stddef.h>
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include "input_output.h"
#include "Array_of_Nodes.h"
#include "Array_int.h"
#include "Array_of_Sums.h"
#define MIN(a, b) ((a) < (b) ? (a) : (b))
  /*!< Computes the minimum of \a a and \a b. */
/**
 * @brief share memory between threads
 * @param array_of_nodes nodes of information
 * @param sieve an array of bools mask for a sieve.
 * @param primes an array of ints with primes.
 * @param can_access_index mutex to access work_unit_index.
 * @param work_unit_index index of the next work unit.
 * @param max the maximum number received
 */
typedef struct {
  array_of_nodes_t *array_of_nodes;
  pthread_mutex_t can_access_index;
  int64_t work_unit_index;
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
/**
* @brief returns the index of the current work unit to be processed and
* decreases the number of work units to be processed
* @param shared_mem the shared memory
* @returns int the index of the current work unit.
*/
int64_t get_next_work_unit(shared_mem_t *shared_mem);
#endif

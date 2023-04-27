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
#define MIN(a, b) ((a) < (b) ? (a) : (b))
  /*!< Computes the minimum of \a a and \a b. */
/**
 * @brief share memory between threads
 * @var array_of_nodes nodes of information ///@param 
 * @var sieve an array of bools mask for a sieve.
 * @var primes an array of ints with primes.
 * @var D the amount of work units
 * @var max the maximum number received
 */
typedef struct {
  array_of_nodes_t *array_of_nodes;
  bool* sieve;
  array_int_t * primes;
  int64_t D;  // work units
  int64_t max;
} shared_mem_t;
/**
 * @brief private memory of a threads
 * @var share_mem shared memory between threads
 * @var start the start index of the block of continuous memory to work on
 * @var finish the end index of the block of continuous memory to work on
 * @var thread_D the amount of work units assigned to the thread
 */
typedef struct {
  shared_mem_t *share_mem;
  int64_t id;
  int64_t start;
  int64_t finish;
  int64_t thread_D;
} thread_mem_t;
/**
 * @brief inits the shared memory
 * @param array_of_nodes nodes of information
 * @param sieve an array of bools mask for a sieve.
 * @param primes an array of ints with primes.
 * @param D the amount of work units
 * @param max the maximum number received
 */
void shared_mem_init(shared_mem_t *share_mem, array_of_nodes_t *array_of_nodes,
  bool* sieve, array_int_t * primes, int64_t D, int64_t max);
/**
 * @brief Destroys the shared memory
 * @param share_mem the shared memory to destroy
 */
void shared_mem_destroy(shared_mem_t *share_mem);
/**
* @brief finds the start index of the work unit of a thread
* @param i the id of the thread
* @param D the number of work units
* @param w the amount of threads
* @return the start index of the work unit of a thread
*/
int64_t start(int64_t i, int64_t D, int64_t w);
/**
* @brief finds the finish index of the work unit of a thread
* @param i the id of the thread
* @param D the number of work units
* @param w the amount of threads
* @return the finish index of the work unit of a thread
*/
int64_t finish(int64_t i, int64_t D, int64_t w);
/**
* @brief distrubutes the work units among the threads and sets the private 
* memory of each thread
* @param thread_count the amount of threads
* @param mem the array of private memory
* @param int D the number of work units
*/
void distribute(shared_mem_t *share_mem, int64_t thread_count,
   thread_mem_t *mem);
#endif

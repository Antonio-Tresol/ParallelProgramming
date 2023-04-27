// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef GOLD_BACH_H
#define GOLD_BACH_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include "Array_int.h"
#include "Array_of_Sums.h"
#include "Array_of_Nodes.h"
#include "threads.h"
#define ABS(a) ((a) < (0) ? (a*-1) : (a))
/*!< Computes the absolute value of  \a a. */
/**
 * @brief computes the goldbach sums a number
 * @param primes the array of primes
 * @param node the node of information with the odd number
 * @param sieve the sieve of atkin
 * @param max the max number recieved
 * @details this procedure is the one that is called from the main program, it
 * checks if the number is even or odd and calls the corresponding procedure
 */
void goldbach_sums(node_t* node, array_int_t* primes, bool* sieve,
   int64_t max);
/**
  * @brief finds the prime numbers between 2 and n
  * @param limit the maximum number to find the primes
  * @param sieve an array of booleans to fill with the sieve
  * @return an array of primes (a mask)
  * @details: part of this code is based Bhojasia, M. (2022, May 16) and 
  * geeksforgeeks.org  see more in bibliography in main README
  * note: in case of this sieve algorithm not working, Eratosthenes sieve will 
  * be used
  */
void Atkin_sieve(bool* sieve, uint64_t limit);
/**
 *@brief fills the array of primes with the first n primes
 *@param sieve a boolean array of primes
 *@param limit the maximum number
 *@param primes the array of int to fill
 */
void fill_array_with_primes(bool* sieve, array_int_t* primes, uint64_t limit);
/**
* @brief computes the goldbach sums of a continuous block of nodes
* assigned to a thread
* @param mem the memory of the thread
*/
void* goldbach(void* mem);
/**
* @brief makes a team of threads, gives them their private memory
* and puts them to compute the goldbach sums of their work units
* @param mem shared memory
* @param thread_count the amount of threads
*/
void compute_goldbach(shared_mem_t* mem, int64_t thread_count);
/**
 * @brief Get the total number of sums object
 * 
 * @param array_of_nodes 
 * @return int64_t 
 */
int64_t get_total_number_of_sums(array_of_nodes_t* array_of_nodes);
#endif

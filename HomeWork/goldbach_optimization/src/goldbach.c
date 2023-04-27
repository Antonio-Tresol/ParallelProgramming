// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include "goldbach.h"
#include <stdio.h>
// procedure sieve_atkin(an array of booleans, an int limit):
void Atkin_sieve(bool *sieve, uint64_t limit) {
  if (limit <= 5) {
    return;
  }
  // int wlimit := square root of limit
  uint64_t wlimit = (uint64_t)sqrtf((long double)limit);
  uint64_t i;
  uint64_t j;
  uint64_t x;
  uint64_t y;
  uint64_t n;
  // for x := 1 to wlimit do
  for ( x = 1; x <= wlimit; x++ ) {
    // for y := 1 to wlimit do
    for ( y = 1; y <= wlimit; y++ ) {
      // n := 4x^2 + y^2
      n = 4 * x * x + y * y;
      /*
      if n <= limit and (n mod 12 = 1 or n mod 12 = 5) then
            sieve[n] := true
         end if 
      */
      if ( n <= limit && (n % 60 == 1 || n % 60 == 13 || n % 60 == 17 ||
       n % 60 == 29 || n % 60 == 37 || n % 60 == 41 || n % 60 == 49 ||
       n % 60 == 53)) {
            sieve[n] = !sieve[n];
            }
            /*
             n := 3x^2 + y^2
             if n <= limit and n mod 12 = 7 then
               sieve[n] := true
             end if
            */
            n = 3 * x * x + y * y;
            if ( n <= limit && (n % 60 == 7 || n % 60 == 19 || n % 60 == 31 ||
             n % 60 == 43) ) {
                sieve[n] = !sieve[n];
            }
            /*
            n := 3x^2 - y^2
            if x > y and n <= limit and n mod 12 = 11 then
              sieve[n] := true
            */
            n = 3 * x * x - y * y;
            if ( x > y && n <= limit && (n % 60 == 11 || n % 60 == 23 ||
             n % 60 == 47 || n % 60 == 59) ) {
                sieve[n] = !sieve[n];
            }
        }
    }
    /*
    for i := 5 to wlimit do
      if sieve[i] = true then
         for j := 1 until j*i^2 <= limit do
            sieve[j*i^2] := false
         end for
      end if
    end for
    */
    for ( i = 5; i <= wlimit; i++ ) {
        if ( sieve[i] == 1 ) {
            for ( j = 1; j * i * i <= limit; j++ ) {
                sieve[j * i * i] = 0;
            }
        }
    }
    /*
    set sieve[2] to true
    set sieve[3] to true
    set sieve[5] to true
   */
    sieve[5] = 1;
    sieve[1] = 0;
    sieve[0] = 0;
    sieve[2] = 1;
    sieve[3] = 1;
    sieve[4] = 0;
}
// procedure get_array_of_primes(a Sieve S, array of primes P):
void fill_array_with_primes(bool *sieve, array_int_t *primes, uint64_t limit) {
    array_int_init(primes);
    // add 2 and 3 to array of primes P

    // for i := 1 to S.limit do
    for ( uint64_t i = 0; i <= limit; i++ ) {
      // if S.sieve[i] is true then
        if ( sieve[i] == 1 ) {
          // P[i] := i
          array_int_append(primes, i);
        }
    }
}
/**
 * @brief computes the goldbach sums of a even number n and stores them in an 
 * array of strings within the node of information
 * @param primes the array of primes
 * @param node the node of information that stores the even number
 * @param sieve the sieve  atkin
 * @param max the maximum number passed by the user
 * @details: part of this code is base on examples of code from geekforgeeks.org
 * with modifications made by the author to optimize time complexity.
 * see main README for more information.
 */
// procedure goldbach_sums_even(Node A, array of primes P, a sieve S):
void goldbach_sums_even(node_t* node, array_int_t* primes, bool* sieve,
  int64_t max) {
    // saveSums := make a boolean flag
  bool save_Sums = false;
  int64_t i;
  int64_t result;
  int64_t abs_num;
  uint64_t abs_number_log_number;
  int64_t for_limit = 0;
  node->array_of_sums = (array_of_sums_t*)calloc(1, sizeof(array_of_sums_t));
  array_of_sums_init(node->array_of_sums);
  /*
  if A.number < 0 then
      saveSums := true
   end if
  */
  if ( node->number < 0 ) {
    save_Sums = true;
  }
  abs_num = ABS(node->number);
  abs_number_log_number = (abs_num == 6 ? 3 :
    abs_num/logl((long double) abs_num));
  if (abs_number_log_number > primes->count) {
    for_limit = primes->count;
  } else {
    for_limit = abs_number_log_number;
  }
  // for i := 1 until |A.number|/ln(|A.number|) do
  for (i = 0; i < for_limit; i++) {
    int64_t prime = primes->elements[i];
    // int  result := |A.number| - P[i]
    result = abs_num - prime;
    // if result < P[i] then
    if (result < prime) {
      // break
      break;
    }
    // End if
    // if S[result] is true then
    if ( result <= max ) {
      if (sieve[result]) {
        //  A.numberOfSums := A.numberOfSums + 1
        node->number_of_sums++;
        // if saveSums is true then
        if (save_Sums) {
          // Add  result and P[i] to A.arrayOfSums as an Array
          array_int_t* sum = (array_int_t*)calloc(1, sizeof(array_int_t));
          array_int_init(sum);
          array_int_append(sum, prime);
          array_int_append(sum, result);
          array_of_sums_append(node->array_of_sums, sum);
          free(sum);
        }
      }
    }
  }
}
/**
 * @brief computes the goldbach sums of a odd number n and stores them in an 
 * array of strings within the node of information
 * @param primes the array of primes
 * @param node the node of information that stores the even number
 * @param sieve the sieve  atkin
 * @param max the maximum number passed by the user
 * @details: part of this code is base on examples of code from geekforgeeks.org, 
 * with modifications made by the author to optimize time complexity.
 * see main README for more information.
 */
// procedure goldbach_sums_odd(a Node A, array of primes P, a sieve S):
void goldbach_sums_odd(node_t* node, array_int_t* primes, bool* sieve,
  int64_t max) {
  // SaveSums := make a boolean flag
  bool save_Sums = false;
  node->array_of_sums = (array_of_sums_t*)calloc(1, sizeof(array_of_sums_t));
  array_of_sums_init(node->array_of_sums);
  uint64_t i;
  uint64_t j;
  int64_t first;
  int64_t second;
  int64_t result;
  uint64_t abs_num;
  uint64_t abs_number_log_number;
  uint64_t for_limit = 0;
  // if A.number < 0 then
  if (node->number < 0) {
    // saveSums := true
    save_Sums = true;
  }
  abs_num = ABS(node->number);
  abs_number_log_number = (abs_num == 6 ? 3 :
    abs_num/logl((long double) abs_num));
  // int limit = min(|A.number|/ln(A.number), P.length)
  if (primes->count < abs_number_log_number) {
    for_limit = primes->count;
  } else {
    for_limit = abs_number_log_number;
  }
  // for i:= 1 to |A.number|/ln(A.number) do
  for (i = 0; i < for_limit; i++) {
    // first := P[i]
    first = primes->elements[i];
    // for j:= i to |A.number|/ln(A.number) do
    for (j = i; j < for_limit; j++) {
      // second := P[j]
      second = primes->elements[j];
      // result := A.number - first - second
      result = abs_num - first - second;
      // if result < second then
      if (result < second) {
        // break
         break;
      }
      // End if
      if ( result <= max ) {
      //  if S[result] is true and result >= first and result >= second then
        if ( sieve[result] && result >= first ) {
          // A.numberOfSums := A.numberOfSums + 1
          node->number_of_sums++;
          // if saveSums is true then
          if (save_Sums) {
            // Add first, second and result to A.arrayOfSums as an Array
            array_int_t* sum = (array_int_t*) calloc(1, sizeof(array_int_t));
            array_int_init(sum);
            array_int_append(sum, first);
            array_int_append(sum, second);
            array_int_append(sum, result);
            array_of_sums_append(node->array_of_sums, sum);
            free(sum);
          }
        }
      }
    }
  }
}
// procedure goldbach_sums(a Node A, array of primes P, a sieve S):
void goldbach_sums(node_t* node, array_int_t* primes, bool* sieve,
  int64_t max) {
  // if A.number > upperLimit
  if ( node->number != 0 ) {
    // else if A.number is even then
    if ( node->number % 2 == 0 ) {
      // goldbach_sums_even(A, P, S)
      goldbach_sums_even(node, primes, sieve, max);
    //   else
    } else {
      // goldbach_sums_odd(A, P, S)
      goldbach_sums_odd(node, primes, sieve, max);
    }
  }
}
// procedure goldbach(thread_memory mem):
void* goldbach(void* mem) {
  // A := mem.A
  shared_mem_t* shared_mem = (shared_mem_t*)mem;
  // A := mem.A
  array_of_nodes_t* Array = shared_mem->array_of_nodes;
  // P := mem.P
  array_int_t* primes = shared_mem->primes;
  // S := mem.S
  bool* sieve = shared_mem->sieve;
  int64_t max = shared_mem->max;
  int64_t index = -1;
  // while true:
  while (true) {
    // index := getNextWorkUnit(shared_mem)
    index = get_next_work_unit(shared_mem);
    // if index == 0 then
    if (index == -1) {
      // break
      break;
    }
    // goldbach_sums(A[index], P, S)
    goldbach_sums(&Array->elements[index], primes, sieve, max);
  }
  // return
  return NULL;
}
// procedure compute_goldbach(Theads_Private_Mem mem, int thread_count):
void compute_goldbach(shared_mem_t* mem, int64_t thread_count) {
  // threads := an array of threads[thread_count]
  pthread_t* threads = (pthread_t*)calloc(thread_count, sizeof(pthread_t));
  // for i := 1 to thread_count do
  for (int64_t i = 0; i < thread_count; i++) {
    // threads := create_thread(thread[i], goldbach, mem[i])
    int error = pthread_create(&threads[i], NULL, goldbach, mem);
    if (error) {
      // print in standard error
      fprintf(stderr, "Error creating thread %ld: %s\n", i, strerror(error));
    }
  }
  // for i := 1 to thread_count do
  for (int64_t i = 0; i < thread_count; i++) {
    // join_thread(threads[i])
    int error = pthread_join(threads[i], NULL);
    if (error) {
      // print in standard error
      fprintf(stderr, "Error joining thread %ld: %s\n", i, strerror(error));
    }
  }
  free(threads);
}
int64_t get_total_number_of_sums(array_of_nodes_t* array_of_nodes) {
  int64_t total_number_of_sums = 0;
  for (uint64_t i = 0; i < array_of_nodes->count; i++) {
    total_number_of_sums += array_of_nodes->elements[i].number_of_sums;
  }
  return total_number_of_sums;
}

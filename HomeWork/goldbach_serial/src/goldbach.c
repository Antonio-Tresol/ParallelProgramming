// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include "goldbach.h"
uint64_t nDivLogn_al(uint64_t number) {
  if ( number == 6 ) {
    return 3;
  }
  uint64_t log_number = logl((long double) number);
  uint64_t a = number / log_number;
  return a;
}
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
    sieve[2] = 1;
    sieve[3] = 1;
}

// procedure get_array_of_primes(a Sieve S, array of primes P):
void fill_array_with_primes(bool *sieve, array_int_t *primes, uint64_t limit) {
    array_int_init(primes);
    // add 2 and 3 to array of primes P
    array_int_append(primes, 2);
    array_int_append(primes, 3);
    // for i := 1 to S.limit do
    for ( uint64_t i = 5; i < limit; i++ ) {
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
 * @credits: part of this code is base on examples of code from geekforgeeks.org
 * with modifications made by the author to optimize time complexity.
 * see main README for more information.
 */
// procedure goldbach_sums_even(Node A, array of primes P, a sieve S):
void goldbach_sums_even_main(node_t* node, array_int_t* primes, bool* sieve,
  uint64_t max) {
    // saveSums := make a boolean flag
  bool save_Sums = false;
  uint64_t i;
  uint64_t result;
  uint64_t abs_num;
  uint64_t abs_number_log_number;
  uint64_t for_limit = 0;
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
  abs_num = abs_number(node->number);
  abs_number_log_number = nDivLogn_al(abs_num);
  if (abs_number_log_number > primes->count) {
    for_limit = primes->count;
  } else {
    for_limit = abs_number_log_number;
  }
  // for i := 1 until |A.number|/ln(|A.number|) do
  for (i = 0; i < for_limit; i++) {
    uint64_t prime = primes->elements[i];
    // int  result := |A.number| - P[i]
    result = abs_num - prime;
    // if S[result] is true then
    if ( result <= max ) {
      if (sieve[result] && result >= prime) {
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
 * @credits: part of this code is base on examples of code from geekforgeeks.org, 
 * with modifications made by the author to optimize time complexity.
 * see main README for more information.
 *
procedure goldbach_sums_odd(a Node A, array of primes P, a sieve S):
   SaveSums := make a boolean flag
   //  check if we have to save the sums
   if A.number < 0 then
      saveSums := true
   end if
   int i;
   int j;
   int k;
   for i:= 1 to |A.number|/ln(A.number) do 
      for j:= i to |A.number|/ln(A.number) do 
         second := P[j] 
         result := A.number - first - second 
         if S[result] is true then  
            A.numberOfSums := A.numberOfSums + 1 
            if saveSums is true then 
               Add first, second and result to A.arrayOfSums as an Array
            end if
         end if
      end for

end procedure
*/
// procedure goldbach_sums_odd(a Node A, array of primes P, a sieve S):
void goldbach_sums_odd_main(node_t* node, array_int_t* primes, bool* sieve,
  uint64_t max) {
  // SaveSums := make a boolean flag
  bool save_Sums = false;
  node->array_of_sums = (array_of_sums_t*)calloc(1, sizeof(array_of_sums_t));
  array_of_sums_init(node->array_of_sums);
  uint64_t i;
  uint64_t j;
  uint64_t first;
  uint64_t second;
  uint64_t result;
  uint64_t abs_num;
  uint64_t abs_number_log_number;
  uint64_t for_limit = 0;
  // if A.number < 0 then
  if (node->number < 0) {
    // saveSums := true
    save_Sums = true;
  }
  abs_num = abs_number(node->number);
  abs_number_log_number = nDivLogn_al(abs_num);
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
      if ( result <= max ) {
      //  if S[result] is true then
        if ( sieve[result] && result >= first && result >= second ) {
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
void goldbach_sums_main(node_t* node, array_int_t* primes, bool* sieve,
  uint64_t max) {
  // if A.number > upperLimit
  if ( node->number != 0 ) {
    // else if A.number is even then
    if ( node->number % 2 == 0 ) {
      // goldbach_sums_even(A, P, S)
      goldbach_sums_even_main(node, primes, sieve, max);
    //   else
    } else {
      // goldbach_sums_odd(A, P, S)
      goldbach_sums_odd_main(node, primes, sieve, max);
    }
  }
}

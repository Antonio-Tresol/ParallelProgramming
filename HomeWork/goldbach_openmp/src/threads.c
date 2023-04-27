// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include "threads.h"
void shared_mem_init(shared_mem_t *share_mem, array_of_nodes_t *array_of_nodes,
  bool* sieve, array_int_t * primes, int64_t max) {
  assert(share_mem);
  share_mem->array_of_nodes = array_of_nodes;
  share_mem->sieve = sieve;
  share_mem->primes = primes;
  share_mem->max = max;
}
void shared_mem_destroy(shared_mem_t *share_mem) {
  assert(share_mem);
  free(share_mem->sieve);
  array_int_destroy(share_mem->primes);
  free(share_mem->primes);
  free(share_mem->array_of_nodes);
  free(share_mem);
}



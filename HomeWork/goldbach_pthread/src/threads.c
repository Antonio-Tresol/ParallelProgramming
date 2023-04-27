// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include "threads.h"
void shared_mem_init(shared_mem_t *share_mem, array_of_nodes_t *array_of_nodes,
  bool* sieve, array_int_t * primes, int64_t D, int64_t max) {
  assert(share_mem);
  share_mem->array_of_nodes = array_of_nodes;
  share_mem->sieve = sieve;
  share_mem->primes = primes;
  share_mem->D = D;
  share_mem->max = max;
}
void shared_mem_destroy(shared_mem_t *share_mem) {
  assert(share_mem);
  free(share_mem->sieve);
  array_int_destroy(share_mem->primes);
  free(share_mem->primes);
  array_of_nodes_destroy(share_mem->array_of_nodes);
  free(share_mem->array_of_nodes);
  free(share_mem);
}
// procedure start(int i, int D, int w):
int64_t start(int64_t i, int64_t D, int64_t w) {
  // int start_index = i * ( D / w ) + min (i, D mod w)
  int64_t start = i * (D / w) + MIN(i, D % w);
  // return start_index
  return start;
}
// procedure finish(int i, int D, int w):
int64_t finish(int64_t i, int64_t D, int64_t w) {
  // int end_index = start(i + 1, D, w)
  int64_t end = start(i + 1, D, w);
  // return end_index
  return end;
}
// procedure distribute(Shared_Memory shared_mem, int thread_count,
//                      Theads_Private_Mem mem):
void distribute(shared_mem_t *share_mem, int64_t thread_count,
  thread_mem_t *mem) {
  // for i := 1 to thread_count do
  for (int64_t i = 0; i < thread_count; i++) {
    // mem[i].shared_mem := shared_mem
    mem[i].share_mem = share_mem;
    // mem[i].id := i - 1
    mem[i].id = i - 1;
    // mem[i].start_index := start(i, share_mem.D, thread_count)
    mem[i].start = start(i, share_mem->D, thread_count);
    // mem[i].finish_index := finish(i, share_mem.D, thread_count)
    mem[i].finish = finish(i, share_mem->D, thread_count);
    // mem[i].thread_D := mem[i].finish_index - mem[i].start_index
    mem[i].thread_D = mem[i].finish - mem[i].start;
  }
}

// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include "threads.h"
void shared_mem_init(shared_mem_t *share_mem, array_of_nodes_t *array_of_nodes,
  bool* sieve, array_int_t * primes, int64_t max) {
  assert(share_mem);
  share_mem->array_of_nodes = array_of_nodes;
  share_mem->sieve = sieve;
  share_mem->primes = primes;
  pthread_mutex_init(&share_mem->can_access_index, NULL);
  share_mem->work_unit_index = (int64_t) array_of_nodes->count- 1;
  share_mem->max = max;
}
void shared_mem_destroy(shared_mem_t *share_mem) {
  assert(share_mem);
  free(share_mem->sieve);
  array_int_destroy(share_mem->primes);
  free(share_mem->primes);
  array_of_nodes_destroy(share_mem->array_of_nodes);
  free(share_mem->array_of_nodes);
  pthread_mutex_destroy(&share_mem->can_access_index);
  free(share_mem);
}
// procedure getNextWorkUnit(Shared_Memory shared_mem):
int64_t get_next_work_unit(shared_mem_t *shared_mem) {
  assert(shared_mem);
  // int index := 0
  int64_t index = -1;
  // lock(shared_mem.can_access_index)
  pthread_mutex_lock(&shared_mem->can_access_index);
  // if shared_mem.work_unit_index > 0 then
  if ( shared_mem->work_unit_index >= 0 ) {
    // index := shared_mem.work_unit_index
    index = shared_mem->work_unit_index;
    // shared_mem.currentWorkUnit := shared_mem.currentWorkUnit - 1
    shared_mem->work_unit_index--;
  }
  // unlock(shared_mem.can_access_index)
    pthread_mutex_unlock(&shared_mem->can_access_index);
  // return index
  return index;
}


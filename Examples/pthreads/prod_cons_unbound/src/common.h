
#ifndef COMMON_H
#define COMMON_H

#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"

enum {
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_UNIT_COUNT,
  ERR_PRODUCER_COUNT,
  ERR_CONSUMER_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};
/**
 * @brief share data between threads
 * @details the data is shared between a producer and a consumer. This implementation
 * is unbounded, so the producer can produce items and put them in the buffer. 
 * for that we used the thread safe queue implementation
 * has a mutex to protect the next unit to be produced
 * and next unit to be consumed
 */
typedef struct simulation {
  size_t unit_count;
  size_t producer_count;
  size_t consumer_count;
  useconds_t producer_min_delay;
  useconds_t producer_max_delay;
  useconds_t consumer_min_delay;
  useconds_t consumer_max_delay;

  queue_t queue;
  pthread_mutex_t can_access_next_unit; //protects the next_unit variable
  size_t next_unit;
  sem_t can_consume;
  pthread_mutex_t can_access_consumed_count;
  size_t consumed_count;
} simulation_t;

useconds_t random_between(useconds_t min, useconds_t max);

#endif  // COMMON_H


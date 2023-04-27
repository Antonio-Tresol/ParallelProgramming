// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "consumer.h"

void* consume(void* data) {
  thread_memory_t* mem = (thread_memory_t*)data;
  simulation_t* simulation = mem->simulation;
  mem->self = pthread_self();
  while (true) {
    // lock(can_access_consumed_count)
    // this mutex is used to protect the consumed_count variable
    // so that only one thread can access it at a time
    // so that we don't have a condition race
    pthread_mutex_lock(&simulation->can_access_consumed_count);
    if (simulation->consumed_count < simulation->unit_count) {
      // Reserve the next product to me
      ++simulation->consumed_count;
    } else {
      // unlock(can_access_consumed_count)
      pthread_mutex_unlock(&simulation->can_access_consumed_count);
      // break while
      break;
    }
    // unlock(can_access_consumed_count)
    pthread_mutex_unlock(&simulation->can_access_consumed_count);

    // wait(can_consume) we use the semaphore so that consumers know when they
    // can consume, this signal is sent by the producer
    sem_wait(&simulation->can_consume);

    size_t value = 0;
    queue_dequeue(&simulation->queue, &value);
    printf("\tConsuming %zu\n", value);
    usleep(1000 * random_between(simulation->consumer_min_delay
      , simulation->consumer_max_delay));
  }

  return NULL;
}

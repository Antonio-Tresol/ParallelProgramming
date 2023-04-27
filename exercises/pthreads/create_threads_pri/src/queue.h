// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
/// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// TODO(all): Implement a thread-safe queue

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>
/**
 * @brief a node of a queue
 * 
 */
typedef struct queue_node {
  size_t data;
  struct queue_node* next;
} queue_node_t;
/**
 * @brief a thread-safe queue
 * @details has a mutex to protect the queue
 */
typedef struct {
  pthread_mutex_t can_access_queue;
  queue_node_t* head;
  queue_node_t* tail;
} queue_t;

/**
 * @brief initializes a queue
 */
int queue_init(queue_t* queue);
/**
 * @brief destroys a queue
 * @param queue 
 * @return int 
 */
int queue_destroy(queue_t* queue);
/**
 * @brief checks if a queue is empty
 */
bool queue_is_empty(queue_t* queue);
/**
 * @brief enqueues a new node
 * 
 * @param queue itself
 * @param data data to put in the new node
 * @return int 0 if success, otherwise an error code
 *
 */
int queue_enqueue(queue_t* queue, const size_t data);
/**
 * @brief dequeues a node
 * 
 * @param queue itself
 * @param data data to put in the new node
 * @return int 0 if success, otherwise an error code
 */
int queue_dequeue(queue_t* queue, size_t* data);
/**
 * @brief removes all nodes from a queue
 * 
 * @param queue itself
 */
void queue_clear(queue_t* queue);

#endif  // QUEUE_H


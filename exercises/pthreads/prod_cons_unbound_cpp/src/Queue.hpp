// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <queue>
#include <mutex>
template <typename dataType>
/**
 * @brief a wrapper for std::queue with a mutex to protect it
 * 
 * @param dataType is the type of data that the queue will process
 */
class queue {
  typedef std::queue<dataType> baseQueue;

 private:
  baseQueue* myQueue;
  std::mutex* can_access_queue;

 public:
  queue() {
    this -> myQueue = new baseQueue();
    this -> can_access_queue = new std::mutex();
  }

  queue(const queue& other) = delete;
  queue(queue&& other) = delete;
  queue& operator=(const queue& other) = delete;
  queue& operator=(queue&& other) = delete;

  ~queue() {
    delete(this -> myQueue);
    delete(this -> can_access_queue);
  }

  bool isEmpty() {
    this -> can_access_queue -> lock();
      bool result = this -> myQueue -> empty();
    this -> can_access_queue -> unlock();

    return isEmpty;
  }

  void enqueue(dataType data) {
    this -> can_access_queue -> lock();
      this -> myQueue -> emplace(data);
    this -> can_access_queue -> unlock();
  }

  void dequeue(dataType* data) {
    this -> can_access_queue->lock();
      *data = this->myQueue->front();
      this->myQueue->pop();
    this->can_access_queue->unlock();
  }

  void clear() {
  }
};
#endif

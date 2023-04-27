// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>
/**
 * @brief a wrapper for std::thread
 * 
 * @param dataType is the type of data that the thread will process
 */
template <typename dataType>
class thread  {
 protected:
  std::thread* myThread;

 public:
  thread():
  myThread(nullptr) {
  }

  virtual void process(dataType data) = 0;

  void initThread(dataType data) {
    myThread = new class std::thread(&thread::process, this, data);
  }

  ~thread() {
    if (this->myThread == nullptr) {
      return;
    }

    if (!this->myThread->joinable()) {
      return;
    }
    this->myThread->join();
    delete this->myThread;
    this->myThread = nullptr;
  }

  void join() {
    this->myThread->join();
    delete this->myThread;
    this->myThread = nullptr;
  }
};

#endif

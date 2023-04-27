// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef PRODUCER_HPP
#define PRODUCER_HPP
#include "Thread.hpp"
/// Declare four of the rule-of-the-five methods
#include "disableCopy.hpp"
/**
 * @brief a producer abstract class that inherits from thread
 * 
 * @tparam dataType is the type of data that the thread will produce
 */
template <typename dataType>
class producer: public thread<dataType> {
 public:
  producer() {
    this -> myThread = nullptr;
  }

  DISABLE_COPY(producer);
  /// @brief the process method for a producer is to produce data until it 
  /// reaches a the amount of data to produce
  /// @param data 
  void process(dataType data) override {
    simulationData* simulationData = (struct simulationData*) data;
  
    while (true) {
      size_t my_unit = 0;

      simulationData->can_access_next_unit.lock();
      if (simulationData->next_unit <
          simulationData->unit_count) {
          my_unit = ++simulationData->next_unit;
      } else {
        simulationData->can_access_next_unit.unlock();
        break;
      }
      simulationData->can_access_next_unit.unlock();
      usleep(1000 * random_between(simulationData->producer_min_delay,
      simulationData->producer_max_delay));

      simulationData->threadQueue.enqueue(my_unit);
      std::cout << "Produced " << my_unit << std::endl;

      sem_post(&simulationData->can_consume);
    }
  }
};
#endif

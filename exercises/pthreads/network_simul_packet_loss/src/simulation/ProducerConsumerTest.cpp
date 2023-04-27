/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <cstdlib>
#include <iostream>

#include "ProducerConsumerTest.hpp"
#include "ConsumerTest.hpp"
#include "DispatcherTest.hpp"
#include "ProducerTest.hpp"

const char* const usage =
  "Usage: packages consumers prod_delay disp_delay cons_delay\n"
  "\n"
  "  packages    number of packages to be produced\n"
  "  consumers   number of consumer threads\n"
  "  prod_delay  delay of producer to create a package\n"
  "  disp_delay  delay of dispatcher to dispatch a package\n"
  "  cons_delay  delay of consumer to consume a package\n"
  "  loss_prob   probability of lossing a package (1 to 100)\n"
  "\n"
  "Delays are in millisenconds, negatives are maximums for random delays\n";

ProducerConsumerTest::~ProducerConsumerTest() {
  delete this->producer;
  delete this->dispatcher;
  for ( ConsumerTest* consumer : this->consumers )
    delete consumer;
}

int ProducerConsumerTest::start(int argc, char* argv[]) {
  // Parse arguments and store values in object members
  if ( int error = this->analyzeArguments(argc, argv) ) {
    return error;
  }

  // Create objects for the simulation
  // Producer is an object encapsulating the producer thread
  this->producer = new ProducerTest(this->packageCount, this->productorDelay
    , this->consumerCount);
  // Dispatcher is an object encapsulating the dispatcher thread
  this->dispatcher = new DispatcherTest(this->dispatcherDelay);
  // Consumers is a vector of objects encapsulating the consumer threads
  this->dispatcher->createOwnQueue(); // Create the queue for the dispatcher
  // Create each producer
  this->assembler = new AssemblerTest(this->lossProbability, this->consumerCount 
    , this->consumerDelay, this->consumerCount - 1);
  this->assembler->createOwnQueue();
  this->consumers.resize(this->consumerCount);
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index] = new ConsumerTest(this->consumerDelay);
    assert(this->consumers[index]);
    if (index < consumerCount - 1){
      this->consumers[index]->createOwnQueue();
    } else {
      this->consumers[index]->setConsumingQueue(this->assembler->getQueue());
    }
  }
  //  Communicate simulation objects
  //  Producer push network messages to the dispatcher queue
  this->producer->setProducingQueue(this->dispatcher->getConsumingQueue());
  this->assembler->setProducingQueue(this->dispatcher->getConsumingQueue());
  //  Dispatcher delivers to each consumer, and they should be registered
  //  look dispatcher, when you see a network message identify by index + 1
  //  send it to the consumer queue with the same index
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    if (index < this->consumerCount-1) {
      this->dispatcher->registerRedirect(index + 1
        , this->consumers[index]->getConsumingQueue());
    } else {
      this->dispatcher->registerRedirect(this->consumerCount + 1
        , this->assembler->getConsumingQueue());
    }
  }
  
  // Start the simulation
  //each element of the simulations is a thread
  // so we start each thread with the start method
  this->producer->startThread();
  this->dispatcher->startThread();
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    if(consumerCount - 1 > index){
      this->consumers[index]->startThread();
    } else {
      this->assembler->startThread();
    }
  }

  // Wait for objets to finish the simulation
  // producer sends sentinel messages to the dispatcher so that
  // the dispatcher knows when to stop, and the dispatcher sends
  // sentinel messages to the consumers so that they know when to stop
  this->producer->waitToFinish();
  this->dispatcher->waitToFinish();
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
     if(consumerCount - 1 > index){
      this->consumers[index]->waitToFinish();
    } else {
      this->assembler->waitToFinish();
    }
  }
  // Simulation finished
  delete this->assembler;
  return EXIT_SUCCESS;
}

int ProducerConsumerTest::analyzeArguments(int argc, char* argv[]) {
  // 6 + 1 arguments are mandatory
  if ( argc != 7 ) {
    std::cout << usage;
    return EXIT_FAILURE;
  }

  int index = 0;
  this->packageCount = std::strtoull(argv[++index], nullptr, 10);
  this->consumerCount = std::strtoull(argv[++index], nullptr, 10);
  this->productorDelay = std::atoi(argv[++index]);
  this->dispatcherDelay = std::atoi(argv[++index]);
  this->consumerDelay = std::atoi(argv[++index]);
  this->lossProbability = std::atoi(argv[++index]);

  // todo: Validate that given arguments are fine
  return EXIT_SUCCESS;
}

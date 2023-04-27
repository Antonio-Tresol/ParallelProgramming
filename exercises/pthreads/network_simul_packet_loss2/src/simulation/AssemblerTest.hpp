#ifndef ASSEMBLER_TEST_HPP
#define ASSEMBLER_TEST_HPP

#include <iostream>
#include "Assembler.hpp"
#include "NetworkMessage.hpp"
#include "Queue.hpp"
class AssemblerTest  
  :public Assembler<NetworkMessage, NetworkMessage> {
  DISABLE_COPY(AssemblerTest);
 protected: 
  /// Number of messages received
  int lossProbability = 0;
  int consumerCount = 0;
  /// Delay of consumer to consume a package, negative for max random
  int producerDelay = 0;
  /// Number of messages received
  size_t receivedMessages = 0;
  int id = 0;
 public:
  /// Constructor
  explicit AssemblerTest(int lossProbability, int consumerCount, int producerDelay,
    int id)
    :Assembler() {
    this->lossProbability = lossProbability;
    this->consumerCount = consumerCount;
    this->producerDelay = producerDelay;
    this->id = id;
  }

  /// Destructor
  virtual ~AssemblerTest() {
  }
  int run() override;
  /// Consume a product
  void consume(NetworkMessage data) override;
  NetworkMessage createMessageRand() const;
  Queue<NetworkMessage>* getQueue();
};
#endif
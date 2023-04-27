#include "AssemblerTest.hpp"
#include "Util.hpp"
void AssemblerTest::consume(NetworkMessage data) {
  (void)data;
  this->receivedMessages++;
  double random = 1 + Util::random(0, 100);
  if (random > this->lossProbability) {
    Util::sleepFor(this->producerDelay);
    produce(createMessageRand());
    std::cout << "Message lost" << std::endl;
  }
}
int AssemblerTest::run(){
  this->consumeForever();
  return EXIT_FAILURE;
}
Queue<NetworkMessage>*  AssemblerTest::getQueue() {
  return this->consumingQueue;
}
NetworkMessage AssemblerTest::createMessageRand() const {
  // Source is always 1: this producer
  const uint16_t source = this->id;
  // Target is selected by random
  int provisionalTarget = 1;

  while (provisionalTarget == id) {
    provisionalTarget = Util::random(1, this->consumerCount);
  }
  const uint16_t target = provisionalTarget;
  return NetworkMessage(target, source);
}

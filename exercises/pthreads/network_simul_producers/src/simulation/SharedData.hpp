#ifndef SHAREDDATA_HPP
#define SHAREDDATA_HPP
#include <cstddef>
#include <cstdlib>
#include <mutex>

#include "common.hpp"
struct producersShareData {
  // DISABLE_COPY(producersShareData);
  /// mutex to protect the shared data
  std::mutex mutex;
  /// counter of the number of packages left to be produced
  int Count = 0;
  /// constructor
  producersShareData(int count): Count(count) {}
  /// destructor
  ~producersShareData() {}
  /// gets the index of the next package to be produced
  /// decrements the counter
  int getIndex() {
    int index = 0;
    mutex.lock();
    index = this->Count;
    if (this->Count > 0) {
      this->Count--;
    }
    mutex.unlock();
    return index;
  }
};
#endif // SHAREDDATA_HPP
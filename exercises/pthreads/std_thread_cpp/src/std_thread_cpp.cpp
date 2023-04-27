// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE comments
#include "std_thread_cpp.hpp"
bool isNumber(std::string strToCheck);
struct shared_data_t {
  uint64_t thread_count;
};
struct private_data_t {
  uint64_t thread_number;
  shared_data_t* shared_data;
};
int main(int argc, char* argv[]) {
  int64_t thread_count = std::thread::hardware_concurrency();
  if (argc == 2) {
    std::stringstream thread_count_string(argv[1]);
    if (thread_count_string >> thread_count) {
    } else {
      std::cerr << "Invalid number of threads: " << argv[1] << std::endl;
      return 1;
    }
  }
  shared_data_t* shared_data = new shared_data_t;
  try {
    shared_data->thread_count = thread_count;
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    try {
      create_threads(shared_data);
      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = (finish_time.tv_sec - start_time.tv_sec);
      elapsed_time += (finish_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
      std::cout << "Elapsed time: " << elapsed_time << " seconds\n";
    } catch (std::exception& e) {
      std::cerr << "Error creating threads: " << e.what() << std::endl;
      return 1;
    }
  } catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
    delete shared_data;
}
void create_threads(shared_data_t* shared_data) {
  try {
    std::vector<std::thread>* threads = new std::vector<std::thread>(
      shared_data->thread_count);
    std::vector<private_data_t>* private_data = new std::vector <private_data_t> (
        shared_data->thread_count);
    try {
      for (uint64_t i = 0; i < shared_data->thread_count; i++) {
        private_data->at(i).thread_number = i;
        private_data->at(i).shared_data = shared_data;
        threads->at(i) = std::thread(greet, &private_data->at(i));
      }
      std::cout << "hello from main std::thread\n";
      // join std::threads
      for (uint64_t i = 0; i < shared_data->thread_count; i++) {
        threads->at(i).join();
      }
      delete threads;
      delete private_data;
    } catch (std::exception& e) {
      std::cerr << "Error creating threads: " << e.what() << std::endl;
      delete threads;
      delete private_data;
      throw e;
    }
  } catch (std::exception& e) {
    std::cerr << "Error vectors for threads: " << e.what() << std::endl;
    return;
  }
  
  
}
void greet(private_data_t* private_data) {
  std::stringstream greetings;
  // this woreks because 
  greetings << "Hello from thread " << private_data->thread_number + 1 << 
    " of " << private_data->shared_data->thread_count << "\n";
  std::cout << greetings.str() << std::flush;
}

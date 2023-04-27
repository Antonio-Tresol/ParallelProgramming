// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }
  // we make a parallel for loop with the number of threads. 
  // this loop shared the iteration_count variable between the threads
  // and the std::cout is a critical section.
  // the keyword here is for, this is a parallel for loop
  #pragma omp parallel for num_threads(thread_count) \
    default(none) shared(iteration_count, std::cout)
    // another keyword here is default(none) this meeans that omp will not
    // assume that any variable is shared or private unless we specify it.
    // here we use the keyword shared to specify that the variables
    // iteration_count and std::cout are shared between the threads. 
    // the keyword private is used to specify that a variable is private to
    // each thread. here we don't use it because we don't have any private
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    #pragma omp critical(stdout)
    std::cout << omp_get_thread_num() << '/' << omp_get_num_threads()
      << ": iteration " << iteration << '/' << iteration_count << std::endl;
  }
  // parallel for loop has several restrictions:
  // 1. the loop must be a simple for loop, a counter loop
  // this is becuase the loop is divided into chunks and each thread executes
  // a chunk of the loop, it is a static block distribution
}
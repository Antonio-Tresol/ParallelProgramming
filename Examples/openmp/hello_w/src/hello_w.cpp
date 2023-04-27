// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>

#include <iostream>

int main(int argc, char* argv[]) {
  // omp_get_max_threads returns the number of threads that can be created in a
  // system
  int thread_count = omp_get_max_threads();
  if (argc == 2) {
    thread_count = atoi(argv[1]);
  }
  // preprocessor directive telling the main thread to create a thread_count
  // with function num_threads(thread_count).
  #pragma omp parallel num_threads(thread_count)
  {
    #pragma omp critical(stdout)
    // omp_get_thread_num returns the id of the thread that is executing the
    // omp_get_num_threads returns the number of threads that are executing
    std::cout << "Hello from secondary thread " << omp_get_thread_num()
      << " of " << omp_get_num_threads() << std::endl;
  }
}

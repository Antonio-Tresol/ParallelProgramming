// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>
// here we use this method to print the vector. we recieve the vector as a const
// reference to avoid copying the vector and  to avoid modifying the vector.
void print_mapping(const char* type, const std::vector<int>& mapping);

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  int block_size = 0;
  if (argc >= 4) {
    block_size = atoi(argv[3]);
  }
  // we declare a vector with iteration_count elements initialized to 0
  // here we save the mapping of the iterations to the threads
  std::vector<int> mapping(iteration_count);

  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(iteration_count, mapping, block_size)
    // here we are passing the block_size as a shared variable between the
    // threads.
  {
    if (block_size == 0) {
      // the keyword schedule is used to specify how the iterations of the for
      // loop will be distributed among the threads
      // the keyword static is used to specify that the iterations will be
      // distributed among the threads in a Static Block Distribution
      #pragma omp for schedule(static)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("static    ", mapping);
      // here the keyword dynamic is used to specify that the iterations will be
      // distributed among the threads in a dynamic way.
      // their default chunk size is 1
      #pragma omp for schedule(dynamic)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("dynamic   ", mapping);
      // here the keyword guided is used to specify that the iterations will be
      // distributed among the threads in a guided way, this is similar to the
      // dynamic distribution but the size of the blocks is not constant
      // at first the size of the blocks is large and then it is reduced as the
      // iterations are distributed. They ask for chucks as soon as they finish.
      // their default chunk size is 1
      #pragma omp for schedule(guided)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("guided    ", mapping);
    } else {
      // here the keyword static is used to specify that the iterations will be
      // distributed among the threads in a static way using fixed blocks
     // using blocks with the static distribution is similar to using the
     // a static cyclic distribution or static cyclic distribution with chunks
      #pragma omp for schedule(static, block_size)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("static,N  ", mapping);
      // here the keyword dynamic is used to specify that the iterations will be
      // distributed among the threads in a dynamic way using fixed blocks
      // for example if we have 10 iterations and 3 threads and we use a block
      // size of 3, the first thread to get work will get 3 units of work.
      #pragma omp for schedule(dynamic, block_size)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("dynamic,N ", mapping);
      // in the case of the guided distribution the chunk size is not constant
      // but it is calculated as the number of iterations divided by the number
      // of threads and it reduces as the iterations are distributed. When we 
      // pass a block size to the guided distribution the minimum chunk size
      // will be the block size.
      #pragma omp for schedule(guided, block_size)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("guided,N  ", mapping);
    }
  }
}

void print_mapping(const char* type, const std::vector<int>& mapping) {
  std::cout << type;
  for (size_t index = 0; index < mapping.size(); ++index) {
    std::cout << mapping[index] << (index == mapping.size() - 1 ? '\n' : ' ');
  }
}
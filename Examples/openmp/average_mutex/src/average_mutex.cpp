// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  std::vector<double> values;

  double value = 0.0;
  // to parallelize this Read operation we need to use a random access files
  // and not a sequential access file. like that we can make several threads
  // read from different positions in the file.
  while (std::cin >> value) {
    values.push_back(value);
  }

  double sum = 0.0;

  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \
    default(none) shared(values, sum)
  for (size_t index = 0; index < values.size(); ++index) {
    #pragma omp critical(can_add) // here we are using a critical section
    // this is a mutex that is used to make sure that only one thread can
    // access the critical section at a time. This is a way to make sure that
    // the sum variable is not being accessed by more than one thread at a time.
    sum += values[index];
  }

  const double average = values.size() ? sum / values.size() : 0.0;
  std::cout << average << std::endl;
}
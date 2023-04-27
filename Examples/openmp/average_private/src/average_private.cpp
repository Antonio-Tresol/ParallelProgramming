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
  while (std::cin >> value) {
    values.push_back(value);
  }

  double total_sum = 0.0;
  //we make a paraller region
  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(values, total_sum)
  {
    // each thread will have its own sum variable
    // this variable will be store in the thread's stack
    double my_partial_sum = 0.0;

    #pragma omp for schedule(runtime)
    for (size_t index = 0; index < values.size(); ++index) {
      // each thread will add its own partial sum to values they get assigned
      my_partial_sum += values[index];
    }

    #pragma omp critical(can_add)  // finally one thread at a time will add
    // the partial sum to the total sums
    total_sum += my_partial_sum;
  }

  const double average = values.size() ? total_sum / values.size() : 0.0;
  std::cout << average << std::endl;
}
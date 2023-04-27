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

  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(values, total_sum)
  {
    double my_partial_sum = 0.0;

    #pragma omp for schedule(runtime)
    for (size_t index = 0; index < values.size(); ++index) {
      my_partial_sum += values[index];
    }
    // #pragma omp critical(can_add)
    // the atomic directive will make sure that only one thread at a time
    // will perform the operation inside the region. But this will be done
    // through hardware instructions, so it will be faster than the critical
    // it is using the atomic hardware instruction to control concurrency.
    // note that not all operations can be done with atomic, only those that
    // are already implemented in the hardware.Like assignment, addition,
    // subtraction, multiplication, division, bitwise and, bitwise or, bitwise
    // xor, bitwise not, and increment and decrement. see documentation for
    // more details.
    #pragma omp atomic
    total_sum += my_partial_sum;
  }

  const double average = values.size() ? total_sum / values.size() : 0.0;
  std::cout << average << std::endl;
}
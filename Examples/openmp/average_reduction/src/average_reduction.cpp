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

  double sum = 0.0;

  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \
    default(none) shared(values) reduction(+:sum) // reduce is a paradigm
    // from functional programming, it is a way to combine the results of
    // multiple threads into a single value with an operation called
    // reduction mapping. it takes a set of values and combines them into a
    // single value.
    // the sintax is reduction(operation:variable)
  for (size_t index = 0; index < values.size(); ++index) {
    sum += values[index];
  }

  const double average = values.size() ? sum / values.size() : 0.0;
  std::cout << average << std::endl;
}
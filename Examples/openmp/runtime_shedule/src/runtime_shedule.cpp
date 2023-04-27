// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

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

  std::vector<int> mapping(iteration_count);
  // here the shedule keyword is used to specify how the iterations will be
  // distributed. In this case we are using a runtime schedule.
  // this is a keyword that is used to specify that the schedule will be
  // determined at runtime base on the "optimal" mapping for the given
  // problem. In reality this is not the optimal mapping, but it is the 
  // mapping that is assigned in an environment variable before hand use
  // the command export OMP_SCHEDULE="schedule_type, block size" to set.
  // behind the scenes the runtime schedule is making all possible mappings
  // and then it is choosing one base on the default mapping type or the
  // mapping type that is specified in the environment variable.
  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \
    default(none) shared(iteration_count, mapping)
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    mapping[iteration] = omp_get_thread_num();
  }

  #pragma omp single
  print_mapping("runtime   ", mapping);
}

void print_mapping(const char* type, const std::vector<int>& mapping) {
  std::cout << type;
  for (size_t index = 0; index < mapping.size(); ++index) {
    std::cout << mapping[index] << (index == mapping.size() - 1 ? '\n' : ' ');
  }
}
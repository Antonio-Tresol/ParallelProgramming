// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>

int calculate_start(int rank, int end, int workers, int begin);
int calculate_finish(int rank, int end, int workers, int begin);

int main(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;  // rank
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    char process_hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_length = -1;
    MPI_Get_processor_name(process_hostname, &hostname_length);

    if (argc == 3) {
      const int overall_start = atoi(argv[1]);
      const int overall_finish = atoi(argv[2]);

      const int process_start = calculate_start(process_number, overall_finish
        , process_count, overall_start);
      const int process_finish = calculate_finish(process_number, overall_finish
        , process_count, overall_start);
      const int process_size = process_finish - process_start;

      std::cout << process_hostname << ':' << process_number << ": range ["
        << process_start << ", " << process_finish << "[ size " << process_size
        << std::endl;
      // this is our equivalent to the paralle do in the pseudo-code
      // remeber that the parallel do is a parallel region
      // and we hace to set default(none) to avoid the implicit sharing
      // and declare explicitly the variables that we want to share
      // between the threads
      #pragma omp parallel default(none) shared(std::cout, process_hostname) \
        shared(process_number, process_start, process_finish)
      {  // NOLINT(whitespace/braces)
        int thread_start = -1;  // we declare mark values
        int thread_finish = -1;

        #pragma omp for schedule(static) // we use block mapping distribution
        for (int index = process_start; index < process_finish; ++index) {
          // inside this loop we'll have to calculate the start and finish
          // for each thread
          if (thread_start == -1) {
            thread_start = index;
          }
          thread_finish = index;
        }
        ++thread_finish; // we have to add one to the finish because the loop
        // does not include the finish value
        // then we can calculate the size of the range
        const int thread_size = thread_finish - thread_start;
        // and we print the information
        // we use omp critical so that the threads do not print at the same time
        #pragma omp critical(can_print)
        std::cout << '\t' << process_hostname << ':' << process_number << '.'
          << omp_get_thread_num() << ": range [" << thread_start << ", " <<
          thread_finish << "[ size " << thread_size << std::endl;
      }
    } else {
      std::cerr << "usage: hybrid_distr_arg start finish" << std::endl;
    }

    MPI_Finalize();
  }

  return 0;
}
// here we implement the functions calculate_start and calculate_finish
// they receive the rank of the process, the end of the range, the number of 
// workers and the begin of the range, and the begin of the range
/**
 * @brief calculate the start of the range for each process
 * 
 * @param rank of the process
 * @param end of the range
 * @param workers 
 * @param begin of the range
 * @return int 
 */
int calculate_start(int rank, int end, int workers, int begin) {
  const int range = end - begin;
  return begin + rank * (range / workers) + std::min(rank, range % workers);
  // using std::min to avoid negative values
}
// the finish is calculated calculating the start of the next thread
/**
 * @brief calculate the finish of the range for each process
 * 
 * @param rank of the process
 * @param end of the range
 * @param workers 
 * @param begin of the range
 * @return int 
 */
int calculate_finish(int rank, int end, int workers, int begin) {
  return calculate_start(rank + 1, end, workers, begin);
}
// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <omp.h>
#include <iostream>

int main(int argc, char* argv[]) {
  // Initialize MPI
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) { // if MPI_SUCCESS is , that means that MPI host was initialized correctly
    int process_number = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number); // get the process number

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count); 

    char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' }; // get the hostname of the process
    int hostname_length = -1; 
    MPI_Get_processor_name(process_hostname, &hostname_length); // get the hostnae of the process
    // here we use hostname, process count and process number to print the message
    std::cout << "Hello from main thread of process " << process_number
      << " of " << process_count << " on " << process_hostname << std::endl;
    // gets the number of threads in the current machine
    int thread_count = omp_get_max_threads();
    if (argc == 2) {
      // recieves the number of threads from argv
      thread_count = atoi(argv[1]);
    }
    // Set the number of threads to use in the next parallel region
    // within each process
    #pragma omp parallel num_threads(thread_count) default(none) \
      shared(std::cout, process_number, process_count, process_hostname)
      // set the shared variables 
    {  // NOLINT(whitespace/braces)
    // make a critical section to use cout
      #pragma omp critical(stdout)
      // print the message using the thread number, process number, process count and hostname
      std::cout << "\tHello from thread " << omp_get_thread_num() << " of "
        << omp_get_num_threads() << " of process " << process_number
        << " of " << process_count << " on " << process_hostname << std::endl;
    }
    // Finalize the MPI environment.
    MPI_Finalize(); 
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
  }
  return 0;
}
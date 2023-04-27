// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
  // in MPI we do not call the group a team of threads, we call them world of 
  // processes/ 
  // the running enviroment (the communicator)
  // is the one in charge of creating the world of
  // processes
  // Initialize the MPI environment by the communicator
  // MPI intit receives not a copy of the arguments but the original ones
  // so that the MPI can modify them
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    // in this course we will use open the mpi region
    // at the beginning of the main function
    // and close it at the end of the main function
    int process_number = -1;
    // Get the number of processes that I am
    // MPI comm Ranck receives the number of world of processes and a number
    // number to put the process number on/
    // the MPI_COMM_WORLD is the constant with the world number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    // Get the name of the processor (processor name of the host
    char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
    int hostname_length = -1;
    // receive a bufferr to put the name of the processor and the length of the
    // name of the processor. it is like an error code
    MPI_Get_processor_name(process_hostname, &hostname_length);

    std::cout << "Hello from main thread of process " << process_number
      << " of " << process_count << " on " << process_hostname << std::endl;
    // mpi init and mpi finalize create a region o communication
    // between the processes.
    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
  }
  return 0;
}
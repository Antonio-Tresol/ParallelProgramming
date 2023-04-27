// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#define fail(msg) throw std::runtime_error(msg)

void process_values(int process_number, int process_count
  , const char* process_hostname);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
    int hostname_length = -1;
    MPI_Get_processor_name(process_hostname, &hostname_length);

    try {
      const double start_time = MPI_Wtime(); // we get the start time
      process_values(process_number, process_count, process_hostname);
      const double elapsed = MPI_Wtime() - start_time; // we get the elapsed time
      std::cout << process_hostname << ":" << process_number
        << ".m: elapsed time " << elapsed << "s" << std::endl; // we print the elapsed time
    } catch (const std::runtime_error& exception) {
      std::cerr << "error: " << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }

    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}
// we define a function to process the values
// we pass the process number, the process count and the process hostname as parameters
/**
 * @brief process the values. it receives the values from the process 0 and sends them to the other processes
 * 
 * @param process_number the process number
 * @param process_count the amount of processes
 * @param process_hostname the name of the host
 *
 */
void process_values(int process_number, int process_count
  , const char* process_hostname) {
  std::vector<double> values;
  size_t value_count = 0;
  // we read the values from the standard input
  if (process_number == 0) { // if we are the main process we do the reading
    double value = 0.0;
    // we put it in values
    while (std::cin >> value) {
      // we push the value to the values vector
      values.push_back(value);
    }
    // we get the size of the values vector
    value_count = values.size();

    for (int target = 1; target < process_count; ++target) {
      // compilation assert, we check if the computer architecture is 64 bits
      // if it is not we throw an error and stops the compilation
      static_assert(sizeof(value_count) == sizeof(uint64_t)
        , "update MPI_Send data type to match your architecture");\
        // we send the value count to the target process
        // REMEMBER: we are MPI sends and receives messages 
        // in the order they are send them. So we can count that the first message
        // will be the value count
      if (MPI_Send(&value_count, /*count*/ 1, MPI_UINT64_T, target
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send value count");
      }
      //
      // we send the array of values to the target process
      if (MPI_Send(&values[0], value_count, MPI_DOUBLE, target
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send values");
      }
    }
  } else { // if we are not the main process we do the receiving and printing
  // we receive the value count from the main process
    if (MPI_Recv(&value_count, /*capacity*/ 1, MPI_UINT64_T, /*source*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive value count");
    }
    // we resize the values vector to match the value count
    values.resize(value_count);
    // we receive the array of values from the main process
    if (MPI_Recv(&values[0], /*capacity*/ value_count, MPI_DOUBLE, /*source*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive values");
    }
  }
  // we print the values
  for (size_t index = 0; index < values.size(); ++index) {
    std::cout << process_hostname << ":" << process_number << ".m: values["
      << index << "] == " << values[index] << std::endl;
  }
}
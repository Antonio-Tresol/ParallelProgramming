#include <mpi.h>
#include <iostream>
#include <stdexcept>

#include "UniformRandom.hpp"

#define fail(msg) throw std::runtime_error(msg)

void generate_lucky_statistics(int process_number, int process_count);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    try {
      int process_number = -1;
      MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

      int process_count = -1;
      MPI_Comm_size(MPI_COMM_WORLD, &process_count);

      char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      int hostname_length = -1;
      MPI_Get_processor_name(process_hostname, &hostname_length);

      generate_lucky_statistics(process_number, process_count);
    } catch (const std::runtime_error& exception) {
      std::cout << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }
    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}
/** 
 * @brief Generates lucky numbers and statistics for a given process and reduces
 *       the results to the host process. it gets the sum of all lucky numbers,
 *      the minimum and maximum lucky number.
 * @param process_number The number of the process.
 * @param process_count The number of processes.
 *
*/
void generate_lucky_statistics(int process_number, int process_count) {
  // Generate my lucky number
  UniformRandom<int> uniformRandom(process_number); //we make a random number generator for each process
  //we use the process number as the seed for the random number generator
  // we want to generate a random number between 0 and 100
  const int my_lucky_number = uniformRandom.between(0, 100);

  std::cout << "Process " << process_number << ": my lucky number is "
    << my_lucky_number << std::endl;

  int all_min = -1;
  int all_max = -1;
  int all_sum = -1;

  // Update distributed statistics from processes' lucky numbers
  // the reduce function takes the following arguments:
  // 1. the address of the data to be reduced (input)
  // 2. the address of the reduced data (output)
  // 3. the number of elements to be reduced
  // 4. the type of the data to be reduced
  // 5. the operation to be performed
  // 6. the rank of the process that will receive the reduced data
  // 7. the communicator (that is the group of processes that will participate in the reduction)
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_min, /*count*/ 1
    , MPI_INT, MPI_MIN, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce min");
  }
  // here we reduce the maximum lucky number
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_max, /*count*/ 1
    , MPI_INT, MPI_MAX, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce max");
  }
  // here we reduce the sum of all lucky numbers
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_sum, /*count*/ 1
    , MPI_INT, MPI_SUM, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce sum");
  }
  // we report our findings through the host process
  // because it is the only one that has all the data
  // the other processes do not have all min, all max and all sum because
  // we only reduced the data to the host process
  if (process_number == 0) {
    const double all_average = double(all_sum) / process_count;
    std::cout << "Process " << process_number << ": all minimum = "
      << all_min << std::endl;
    std::cout << "Process " << process_number << ": all average = "
      << all_average << std::endl;
    std::cout << "Process " << process_number << ": all maximum = "
      << all_max << std::endl;
  }
}
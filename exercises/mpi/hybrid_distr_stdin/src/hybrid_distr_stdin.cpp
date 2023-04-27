// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
/**
 * @brief Calculate the start of the range for the given processNumber.
 * 
 * @param processNumber the processNumber of the process
 * @param end  the end of the range
 * @param workers  the number of workers
 * @param begin  the begin of the range
 * @return int 
 */
int calcStart(int processNumber, int end, int workers, int begin) {
  const int range = end - begin;
  return begin + processNumber * (range / workers) +
    std::min(processNumber, range % workers);
}
/**
 * @brief Calculate the end of the range for the given processNumber.
 * 
 * @param processNumber  the processNumber of the process
 * @param end  the end of the range
 * @param workers  the number of workers
 * @param begin  the begin of the range
 * @return int 
 */
int calcFinish(int processNumber, int end, int workers, int begin) {
  return calcStart(processNumber + 1, end, workers, begin);
}
int main(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int processNumber = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);

    int processCount = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    char processHostname[MPI_MAX_PROCESSOR_NAME];
    int hostnameLength = -1;
    MPI_Get_processor_name(processHostname, &hostnameLength);
    int overallStart = -1;
    int overallFinish = -1;
    if (argc == 3) {
      overallStart = atoi(argv[1]);
      overallFinish = atoi(argv[2]);
    } else {
      if (processNumber == 0) {
        std::cout << "Please enter the start value: " << std::endl;
        std::cin >> overallStart;
        std::cout << "Please enter the finish value: " << std::endl;
        std::cin >> overallFinish;
        for (int process = 1; process < processCount; ++process) {
          if (MPI_Send(&overallStart, 1, MPI_INT, process, 0, MPI_COMM_WORLD)
            != MPI_SUCCESS) {
            std::cerr << "Error sending start value to process " << process <<
              std::endl;
          }
          if (MPI_Send(&overallFinish, 1, MPI_INT, process, 0, MPI_COMM_WORLD)
            != MPI_SUCCESS) {
            std::cerr << "Error sending finish value to process " << process <<
              std::endl;
          }
        }
      } else {
        if (MPI_Recv(&overallStart, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
          MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          std::cerr << "Error receiving start value from process 0" <<
            std::endl;
        }
        if (MPI_Recv(&overallFinish, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
          MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          std::cerr << "Error receiving finish value from process 0" <<
            std::endl;
        }
      }
    }
    const int processStart = calcStart(processNumber, overallFinish
      , processCount, overallStart);
    const int processFinish = calcFinish(processNumber, overallFinish
      , processCount, overallStart);
    const int process_size = processFinish - processStart;
    std::cout << processHostname << ':' << processNumber << ": range ["
      << processStart << ", " << processFinish << "[ size " << process_size
      << std::endl;
    #pragma omp parallel default(none) \
      shared(std::cout, processNumber, processCount, processHostname, \
      processStart, processFinish, process_size) 
      {   // NOLINT(whitespace/braces)
      int threadStart = -1;
      int threadFinish = -1;
      #pragma omp for
      for (int index = processStart; index < processFinish; ++index) {
        if (threadStart == -1) {
          threadStart = index;
        }
        threadFinish = index;
      }
      ++threadFinish;
      const int threadBlockSize = threadFinish - threadStart;
      #pragma omp critical(stdout)
      std::cout << '\t' << processHostname << ':' << processNumber << '.'
        << omp_get_thread_num() << ": range [" << threadStart << ", " <<
        threadFinish << "[ size " << threadBlockSize << std::endl;
    } 
    MPI_Finalize();
  }
  return 0;
}

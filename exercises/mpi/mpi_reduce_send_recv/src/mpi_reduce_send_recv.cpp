// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <mpi.h>
#include <iostream>
#include <stdexcept>

#include "UniformRandom.hpp"

#define fail(msg) throw std::runtime_error(msg)

void generate_lucky_statistics(int processNumber, int processCount);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    try {
      int processNumber = -1;
      MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);

      int processCount = -1;
      MPI_Comm_size(MPI_COMM_WORLD, &processCount);

      char processHostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      int hostnameLength = -1;
      MPI_Get_processor_name(processHostname, &hostnameLength);

      generate_lucky_statistics(processNumber, processCount);
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

void generate_lucky_statistics(int processNumber, int processCount) {
  // Generate my lucky number
  UniformRandom<int> uniformRandom(processNumber);
  const int myLuckyNumber = uniformRandom.between(0, 100);

  std::cout << "Process " << processNumber << ": my lucky number is "
                          << myLuckyNumber << std::endl;
  int allMin = -1;
  int allMax = -1;
  int allSum = -1;
  int tempNumber = myLuckyNumber;
  // here we make a tree to reduce data by passing the to a father process
  // in a tree structure
  for (int v2_t = pow(2,ceil(log2(processCount)-1)); v2_t > 0; v2_t >>= 1){
    if ((processNumber >= v2_t) && (processNumber < (v2_t << 1)))
    MPI_Send(&tempNumber, 1, MPI_INT, (processNumber-v2_t), 0, MPI_COMM_WORLD);
    else if ((processNumber < v2_t) && (processNumber + v2_t) < processCount) {
      MPI_Recv(&allMax, 1, MPI_INT, (processNumber+v2_t), 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      if (allMax > tempNumber) {
        tempNumber = allMax;
      }
    }
  }
  allMax = tempNumber;
  tempNumber = myLuckyNumber;
  for (int v2_t = pow(2,ceil(log2(processCount)-1)); v2_t > 0; v2_t >>= 1){
    if ((processNumber >= v2_t) && (processNumber < (v2_t << 1)))
    MPI_Send(&tempNumber, 1, MPI_INT, (processNumber-v2_t), 0, MPI_COMM_WORLD);
    else if ((processNumber < v2_t) && (processNumber + v2_t) < processCount) {
      MPI_Recv(&allMin, 1, MPI_INT, (processNumber+v2_t), 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      if (allMin < tempNumber) {
        tempNumber = allMin;
      }
    }
  }
  allMin = tempNumber;
  tempNumber = myLuckyNumber;
  for (int v2_t = pow(2,ceil(log2(processCount)-1)); v2_t > 0; v2_t >>= 1){
    if ((processNumber >= v2_t) && (processNumber < (v2_t << 1)))
      MPI_Send(&tempNumber, 1, MPI_INT, (processNumber-v2_t), 0, MPI_COMM_WORLD);
    else if ((processNumber < v2_t) && (processNumber + v2_t) < processCount) {
      MPI_Recv(&allSum, 1, MPI_INT, (processNumber+v2_t), 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      tempNumber+=allSum;
    }
  }
  allSum = tempNumber;
  MPI_Barrier(MPI_COMM_WORLD);
  if (processNumber == 0) {
    const double allAverage = double(allSum) / processCount;
    std::cout << "Process " << processNumber << ": all minimum = "
      << allMin << std::endl;
    std::cout << "Process " << processNumber << ": all average = "
      << allAverage << std::endl;
    std::cout << "Process " << processNumber << ": all maximum = "
      << allMax << std::endl;
  }
}

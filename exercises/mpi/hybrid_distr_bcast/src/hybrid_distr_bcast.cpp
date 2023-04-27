//  Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
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
 * @return int with the start of the range
 */
int calcStart(int processNumber, int end, int workers, int begin);
/**
 * @brief Calculate the end of the range for the given processNumber.
 * 
 * @param processNumber  the processNumber of the process
 * @param end  the end of the range
 * @param workers  the number of workers
 * @param begin  the begin of the range
 * @return int 
 */
int calcFinish(int processNumber, int end, int workers, int begin);

int main(int argc, char* argv[]) {
  //  Inicialize MPI
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int processNumber = -1;  // processNumber
    MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);

    //  Get the number of nodes devices
    int processCount = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    char processHostname[MPI_MAX_PROCESSOR_NAME];
    int hostnameLength = -1;
    MPI_Get_processor_name(processHostname, &hostnameLength);

    //  Start and end
    int start = 0;
    int end = 0;

    if (processNumber == 0) {
      std::cout << "Introduce start index\n";
      std::cin >> start;
      std::cout << "Instroduce end index\n";
      std::cin >> end;
    }

    if (MPI_Bcast(&start, /*count*/ 1, MPI_UINT64_T, /*root*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
      std::cout << "Error sending parameters\n";
    }

    if (MPI_Bcast(&end, /*count*/ 1, MPI_UINT64_T, /*root*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
      std::cout << "Error sending parameters\n";
    }

    double startTime = MPI_Wtime();
    //  Mapping each start of each process
    const int processStart = calcStart(processNumber, end
    , processCount, start);
    //  Mapping each finish of each process
    const int processFinish = calcFinish(processNumber, end
    , processCount, start);
    //  Once calculating start and finish, get size
    const int processBlockSize = processFinish - processStart;

    std::cout << processHostname << ':' << processNumber << ": range ["
    << processStart << ", " << processFinish << "[ size " << processBlockSize
    << std::endl;

    #pragma omp parallel default(none) shared(std::cout, processHostname) \
    shared(processNumber, processStart, processFinish)
    {  // NOLINT(whitespace/braces)
      int threadStart = -1;
      int threadFinish = -1;

      #pragma omp for schedule(static)
      for (int index = processStart; index < processFinish; ++index) {
        if (threadStart == -1) {
          threadStart = index;
        }
        threadFinish = index;
      }
      ++threadFinish;
      const int threadBlockSize = threadFinish - threadStart;
      #pragma omp critical(can_print)
      std::cout << '\t' << processHostname << ':' << processNumber << '.'
      << omp_get_thread_num() << ": range [" << threadStart << ", " <<
      threadFinish << "[ size " << threadBlockSize << std::endl;
    }

    double endTime = MPI_Wtime();
    std::cout << "Process " << processNumber << " in time "
    << endTime - startTime << "s\n";

    MPI_Finalize();
  }

  return 0;
}

int calcStart(int processNumber, int end, int workers, int begin) {
  const int range = end - begin;
  return begin + processNumber * (range / workers) + std::min(processNumber, range % workers);
}

int calcFinish(int processNumber, int end, int workers, int begin) {
  return calcStart(processNumber + 1, end, workers, begin);
}

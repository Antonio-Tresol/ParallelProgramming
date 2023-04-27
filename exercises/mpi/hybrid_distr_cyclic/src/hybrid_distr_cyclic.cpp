// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  //  Inicialize MPI
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int processNumber = -1;  // processNumber
    MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
    //  Get the number processes in the communicator world
    int processCount = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    char processHostname[MPI_MAX_PROCESSOR_NAME];
    int hostnameLength = -1;
    MPI_Get_processor_name(processHostname, &hostnameLength);

    //  Start and end
    int start = 0;
    int end = 0;
    std::vector<int> indexList;

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
    for (int index = processNumber; index < end; index += processCount) {
      indexList.push_back(index);
    }
    // compute block size
    const int processBlockSize = indexList.size();

    std::cout << processHostname << ':' << processNumber << ": ";
    for (int i = 0; i < processBlockSize; ++i) {
      if (i == 0) {
        std::cout << indexList[i];
      } else {
        std::cout << ", " << indexList[i];
      }
    }
    std::cout << " size " << processBlockSize << std::endl;
    #pragma omp parallel default(none) shared(std::cout, processHostname) \
    shared(processNumber, processBlockSize, indexList)
    {  // NOLINT(whitespace/braces)
      #pragma omp for schedule(static)
      /// distributing the work cyclically
      for (int index = 0; index < processBlockSize; ++index) {
        #pragma omp critical  // pragma omp critical so that only one thread
        //  prints at a time
        std::cout << '\t' << processHostname << ':' << processNumber << '.'
        << omp_get_thread_num() << indexList[index] << std::endl;
      }
    }
    double endTime = MPI_Wtime();
    std::cout << "Process " << processNumber << " in time "
    << endTime - startTime << "s\n";
    MPI_Finalize();
  }

  return 0;
}
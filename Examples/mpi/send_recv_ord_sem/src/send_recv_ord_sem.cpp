// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept> // standard exceptions

// DistributedExeption(process_number, exception_code, msg)
#define fail(msg) throw std::runtime_error(msg) //macro fail to throw exceptions

void greet(int process_number, int process_count, const char* process_hostname);

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
      greet(process_number, process_count, process_hostname);
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
/**
 * @brief greets from processes using send and receive as semaphores to 
 * impose order
 * 
 * @param process_number the number of the process
 * @param process_count the number of processes
 * @param process_hostname the name of the host
 */
void greet(int process_number, int process_count
    , const char* process_hostname) {
  const int previous_process = (process_count + process_number - 1)
    % process_count; // previous process
  const int next_process = (process_number + 1) % process_count;
  bool can_print = true; // boolean flag to send to no next process

  if (process_number > 0) {
    // receive(&can_print, 1, previous_process)
    // here we use mpi_recv to receive the boolean flag, we put it in the
    // variable can_print, we receive only one element, and we receive from
    // we indicate taht we are receiving a mpi_c_bool, and we receive from
    // the previous process, we also put mpi_status_ignore because we are not
    // interested in the status of the message or saving it
    // so if we are process 1, we receive from process 0
    // if we received successfully, we print the message, if not, we throw
    // a distributed exception
    if (MPI_Recv(&can_print, /*count*/ 1, MPI_C_BOOL, previous_process
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive message"); // distributed exception
    }
  }

  std::cout << "Hello from main thread of process " << process_number
    << " of " << process_count << " on " << process_hostname << std::endl;

  // send(can_print, 1, next_process);
  // here we use mpi_send to send the boolean flag, we send it from the
  // variable can_print, we send only one element, and we send to
  // the next process of type mpi_c_bool 
  // so if we are process 0, we send to process 1
  // if we sent successfully, we pass, if not, we throw a distributed
  // exception
  // next process belongs to my team of processes
  if (MPI_Send(&can_print, /*count*/ 1, MPI_C_BOOL, next_process, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("could not send message");// distributed exception
  }
}
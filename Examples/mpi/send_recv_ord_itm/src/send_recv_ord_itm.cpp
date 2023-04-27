// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#define fail(msg) throw std::runtime_error(msg)

const size_t MESSAGE_CAPACITY = 512; // we define a maximum message size

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
    // handling possible errors
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
 * @brief greets from process 0 to all other processes sending a message to be printed
 * by the main process. it does this by using send and receive functions to impose
 * a specific order to the messages
 * 
 * @param process_number the number of the process
 * @param process_count the number of processes
 * @param process_hostname the name of the host
 *
 */
void greet(int process_number, int process_count
    , const char* process_hostname) {
  std::stringstream buffer; // we cre4ate a buffer to store the message using a string stream
  // we then write the message to the buffer
  buffer << "Hello from main thread of process " << process_number 
      << " of " << process_count << " running on " << process_hostname
    << " of " << process_count << " on " << process_hostname;
  // important because we cannot send messages to ourselves because it does not
  // work and it uses full CPU waiting for the message to arrive
  if (process_number != 0) { // if we are not the main process 
    // we send the message to the main process
    const std::string& message = buffer.str(); // we get the message from the buffer
    // and we send it to the main process
    // we send the message.data() using the send function and its length
    if (MPI_Send(message.data(), message.length(), MPI_CHAR, /*target*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      fail("could not send message"); // if we could not send the message we throw an error using fail macro
    }
  } else { // if we are not the main process
  // we print our own message
    std::cout << process_number << " said " << buffer.str() << std::endl;
// and then we receive the messages from the other processes and print them
    for (int source = 1; source < process_count; ++source) {
      std::vector<char> message(MESSAGE_CAPACITY); // we make a vector of chars to store the message
      // of size MESSAGE_CAPACITY
      // to get the char* we use the from the vector we use &message and we access the first element
      // like &message[0] this
      // we dont know the size of the message so we use a buffer, expecting that the message will be
      // smaller than the buffer, here it is MESSAGE_CAPACITY
      if (MPI_Recv(&message[0], MESSAGE_CAPACITY, MPI_CHAR, source
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        fail("could not receive message");
      }
      std::cout << source << " sent " << &message[0] << std::endl;
    }
  }
}
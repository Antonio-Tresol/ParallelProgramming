// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include "ProcessesCoordination.h"
// TAGS to organize the code
#define basic_info 0
#define ask_for_work 1
#define results 2
#define sending_node 3

int compute_goldbach_alone(shared_mem_t* mem, array_of_nodes_t* A, int64_t max,
  array_int_t* P, bool* S, int64_t thread_count) {
  // read the numbers
  max = read_numbers(A);
  // prepare the sieve
  S = (bool*) malloc((max + 1) * sizeof(bool));
  SieveOfEratosthenes(S, max);
  // prepare the primes
  fill_array_with_primes(S, P, max);
  // prepare the shared memory
  shared_mem_init(mem, A, S, P, max);
  // compute the goldbach sums of the numbers
  compute_goldbach(mem, thread_count, 0, A->count);
  // print the results
  int64_t total = get_total_number_of_sums(A);
    printf("Total %" PRIu64 " numbers %" PRId64 " sums\n",
      A->count, total);
  print_goldbach_sums(A);
  return 0;
}

int compute_goldbach_together(shared_mem_t* mem, array_of_nodes_t* A,
  int64_t max, array_int_t* P, bool* S, int process_number, int process_count,
  int64_t thread_count) {
    // setting up the shared memory
    array_coordinates_t* C = (array_coordinates_t*) calloc(1,
      sizeof(array_coordinates_t));
    array_coordinates_init(C);
    int64_t amount_of_elements = 0;
    // process 0 reads the numbers and sends them to the other processes
    if (process_number == 0) {
      max = read_numbers(A);
      amount_of_elements = A->count;
    }
    // process 0 sendind basic data to all processes
    if (process_number == 0) {
      if (send_base_info(amount_of_elements, max, A, process_count) != 0) {
        return 1;
      }
    } else {  // all processes receive the basic data
      if (receive_base_info(&amount_of_elements, &max, A) != 0) {
        fprintf(stderr, "Error: Receive_base_info_failed in process %d\n",
        process_number);
        return 1;
      }
    }
    S = (bool*) malloc((max + 1) * sizeof(bool));
    SieveOfEratosthenes(S, max);
    fill_array_with_primes(S, P, max);
    shared_mem_init(mem, A, S, P, max);
    // waiting for all processes to be ready
    if (MPI_Barrier(world) != success) {
      fprintf(stderr, "Error: MPI_Barrier failed in process %d\n",
        process_number);
      return 1;
    }
    int64_t request_id = 0;
    int64_t begin = 0;
    int64_t end = 0;
    int64_t max_request_num = process_count * 2;
    if (process_number == 0) {
      // process 0 is in charge of sending the requests ids to the other
      // processes
      while (request_id < max_request_num) {
        int process = -1;
        // receive process number ready
        if (MPI_Recv(&process, 1, MPI_INT, MPI_ANY_SOURCE, ask_for_work, world,
          ignore) != success) {
          fprintf(stderr, "Error: Master MPI_Recv failed in its distribution");
          return 1;
        }
        // send request id
        if (MPI_Send(&request_id, 1, MPI_INT64_T, process, ask_for_work, world)
          != success) {
          fprintf(stderr, "Error: Master MPI_Send failed in its distribution");
          return 1;
        }
        // receive updated request id
        if (MPI_Recv(&request_id, 1, MPI_INT64_T, process, ask_for_work, world,
         ignore) != success) {
          fprintf(stderr, "Error: Master MPI_Recv failed in its distribution");
          return 1;
        }
        request_id++;
      }
      // after request id is max_request_num, send to all processes
      for (int process = 1; process < process_count; process++) {
         if (MPI_Send(&request_id, 1, MPI_INT64_T, process, ask_for_work, world)
          != success) {
          fprintf(stderr, "Error: Master MPI_Send failed while sending stop");
          return 1;
        }
      }
    } else {  // process_number != 0
      while (request_id < max_request_num) {
        // tell master process that you are ready
        if (MPI_Send(&process_number, 1, MPI_INT, 0, ask_for_work, world)
          != success) {
          fprintf(stderr, "Error: MPI_Send failed in process %d\n",
            process_number);
          return 1;
        }
        // receive request id from master
        if (MPI_Recv(&request_id, 1, MPI_INT64_T, 0, ask_for_work, world,
          ignore) != success) {
          fprintf(stderr, "Error: MPI_Recv failed in process %d\n",
            process_number);
          return 1;
        }
        // if request id less than max_request_num, compute
        if (request_id < max_request_num) {
          // get your begin and end
          get_work_units(request_id, max_request_num,
            amount_of_elements, &begin, &end);
          if (begin != end) {
          // send the request id to master
            if (MPI_Send(&request_id, 1, MPI_INT64_T, 0, ask_for_work, world)
              != success) {
              fprintf(stderr, "Error: Process %d MPI_Send request id failed\n",
                process_number);
              return 1;
            }
            // add the coordinates to the coordinates array
            coordinates_t new_cordinates = {begin, end};
            if (array_coordinates_append(C, new_cordinates) != 0) {
              fprintf(stderr,
              "Error: array_coordinates_append failed in process %d\n",
              process_number);
              return 1;
            }
          } else {
            request_id = max_request_num;
            if (MPI_Send(&request_id, 1, MPI_INT64_T, 0, ask_for_work, world)
              != success) {
              fprintf(stderr, "Error: Process %d MPI_Send request id failed\n",
                process_number);
              return 1;
            }
          }
          // compute the coordinates
          compute_goldbach(mem, thread_count, begin, end);
        }
      }
    }

    MPI_Barrier(world);
    if (process_number != 0) {
      if (send_results(A, C, process_number) != 0) {
        return 1;
      }
      // printf("Process %d finished, got %ld blocks:\n", process_number,
      //   C->count);
      // for (int i = 0; i < C->count; i++) {
      //   printf("- block [ %ld, %ld ]\n",
      //     C->elements[i].start, C->elements[i].finish);
      // }
    } else {
      if (receive_results(A, process_count) != 0) {
        fprintf(stderr, "Error: Receive_results failed in process %d\n",
          process_number);
        return 1;
      }
      int64_t total = get_total_number_of_sums(A);
      printf("Total %" PRIu64 " numbers %" PRId64 " sums\n",
      A->count, total);
      print_goldbach_sums(A);
    }
    return 0;
  }

int send_base_info(int64_t elements_count, int64_t max, array_of_nodes_t* A,
  int process_count) {
    // send the number of elements to all processes
  for (int process = 1; process < process_count; process++) {
    // send the number of elements
    if (MPI_Send(&elements_count, 1, MPI_INT64_T, process, basic_info, world)
      != success) {
      fprintf(stderr, "Error: MPI_Send failed in send_base_info");
      return 1;
    } 
    // send the max number
    if (MPI_Send(&max, 1, MPI_INT64_T, process, basic_info, world)!= success) {
      return 1;
    }
    // send the number of each node
    for (int64_t node = 0; node < elements_count; node++) {
      if (A->elements[node].number_of_sums != -1) {
        int signal = 1;
        // tell if there is a valid node
        if (MPI_Send(&signal, 1, MPI_INT, process, basic_info, world)
        != success) {
          fprintf(stderr, "Error: MPI_Send failed in send_base_info");
          return 1;
        }
        // send the number
        if (MPI_Send(&A->elements[node].number, 1, MPI_INT64_T, process,
          0, world) != success) {
          fprintf(stderr, "Error: MPI_Send failed in send_base_info");
          return 1;
        }
      } else {
        int signal = 0;
        // tell 
        if ( MPI_Send(&signal, 1, MPI_INT, process, basic_info, world)
        != success ) {
          fprintf(stderr, "Error: MPI_Send failed in send_base_info");
          return 1;
        }
      }
    }
  }
  return 0;
}

int receive_base_info(int64_t* elements_count, int64_t* max,
  array_of_nodes_t* A) {
  if (MPI_Recv(elements_count, 1, MPI_INT64_T, 0, basic_info, world, ignore)
    != success) {
    return 1;
  }
  if (MPI_Recv(max, 1, MPI_INT64_T, 0, basic_info, world, ignore) != success) {
    return 1;
  }
  for (int64_t node = 0; node < *elements_count; node++) {
    int signal = 0;
    if (MPI_Recv(&signal, 1, MPI_INT, 0, basic_info, world, ignore)
    != success) {
      return 1;
    }
    if (signal == 1) {
      int64_t number = 0;
      if (MPI_Recv(&number, 1, MPI_INT64_T, 0, basic_info, world, ignore)
        != success) {
        return 1;
      }
      array_of_nodes_append(A, number);
    } else {
      array_of_nodes_append_void(A);
    }
  }
  return 0;
}

int send_results(array_of_nodes_t* A, array_coordinates_t* C,
  int process_number) {
  // tell master you are ready to send results
  if (MPI_Send(&process_number, 1, MPI_INT, 0, results, world) != success) {
    fprintf(stderr, "Error: MPI_Send failed in send_results");
    return 1;
  }
  int64_t amount_of_blocks = C->count;
  // send the amount of blocks you have
  if (MPI_Send(&C->count, 1, MPI_INT64_T, 0, results, world) != success) {
    fprintf(stderr,
      "Error: Process %d : MPI_Send failed in send_results sending blocks\n",
      process_number);
    return 1;
  }
  // if you have blocks send them
  if (amount_of_blocks == 0) {
    return 0;  // if you have no blocks you are done
  }
  // to send the blocks you need to send the start and finish of each block
  for (int64_t block = 0; block < amount_of_blocks; block++) {
    int64_t begin = C->elements[block].start;
    int64_t end = C->elements[block].finish;
    if (MPI_Send(&begin, 1, MPI_INT64_T, 0, results, world) != success) {
      fprintf(stderr,
      "Error: Process %d : MPI_Send failed in send_results (sending start)",
      process_number);
      return 1;
    }
    if (MPI_Send(&end, 1, MPI_INT64_T, 0, results, world) != success) {
      fprintf(stderr,
      "Error: Process %d : MPI_Send failed in send_results (sending finish)"
      , process_number);
      return 1;
    }
    // now send the node
    for (int64_t node = begin; node < end; node++) {
      if (A->elements[node].number_of_sums != -1) {
        if (send_node(&A->elements[node]) != 0) {
          fprintf(stderr,
            "Error: Process %d : Send Results failed in node %ld",
            process_number, node);
          return 1;
        }
      }
    }
  }
  return 0;
}

int receive_results(array_of_nodes_t* A, int64_t process_count) {
  int process_sending = -1;
  // receive from each process
  for (int process = 1; process < process_count; process++) {
    // receive one the process number to receive results from
    if (MPI_Recv(&process_sending, 1, MPI_INT, process, results, world, ignore)
      != success) {
      fprintf(stderr, "Error: MPI_Recv failed in receive_results");
      return 1;
    }
    int64_t amount_of_blocks = 0;
    // receive the amount of blocks
    if (MPI_Recv(&amount_of_blocks, 1, MPI_INT64_T, process_sending, results,
      world, ignore) != success) {
      return 1;
    }
    // if process_sending have no blocks you are done
    if (amount_of_blocks == 0) {
      continue;
    }
    // if not receive the blocks
    for (int64_t block = 0; block < amount_of_blocks; block++) {
      int64_t begin = 0;
      int64_t end = 0;
      // receive the start and finish of the block
      if (MPI_Recv(&begin, 1, MPI_INT64_T, process_sending, results,
      world, ignore) != success) {
        return 1;
      }
      if (MPI_Recv(&end, 1, MPI_INT64_T, process_sending, results, world,
        ignore) != success) {
        return 1;
      }
      // receive the nodes
      for (int64_t node = begin; node < end; node++) {
        if (A->elements[node].number_of_sums != -1) {  // if is not void
          if (receive_node(&A->elements[node], process_sending) != 0) {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

int send_node(node_t* node) {
  // send the number of sums
  if (MPI_Send(&node->number_of_sums, 1, MPI_INT64_T, 0, sending_node,
    world) != success) {
    return 1;
  }
  // if you have sums send them
  if (node->number < 0) {
    for (int64_t index = 0; index < node->number_of_sums; index++) {
      array_int_t numbers = node->array_of_sums->sums[index];
      uint64_t count = numbers.count;
      // send the count of the array
      if (MPI_Send(&count, 1, MPI_UINT64_T, 0, sending_node, world)
      != success) {
        return 1;
      }
      // send the array one by one
      for (uint64_t i = 0; i < count; i++) {
        int64_t number = numbers.elements[i];
        if (MPI_Send(&number, 1, MPI_INT64_T, 0, sending_node,
          world) != success) {
          return 1;
        }
      }
    }
  }
  return 0;
}
int receive_node(node_t* node, int process_number) {
  // receive the number of sums
  if (MPI_Recv(&node->number_of_sums, 1, MPI_INT64_T, process_number,
    sending_node, world, ignore) != success) {
    return 1;
  }
  // if you have sums receive them
  if (node->number < 0) {
    // allocate memory for the sums
    node->array_of_sums = (array_of_sums_t*) calloc(1, sizeof(array_of_sums_t));
    array_of_sums_init(node->array_of_sums);
    // receive the sums
    for (int64_t index = 0; index < node->number_of_sums; index++) {
      // prepare the array to receive the numbers
      array_int_t* sum = (array_int_t*) calloc(1, sizeof(array_int_t));
      array_int_init(sum);
      uint64_t count = 0;
      // receive the count of the array
      if (MPI_Recv(&count, 1, MPI_UINT64_T, process_number, sending_node,
        world, ignore) != success) {
        return 1;
      }
      // receive the array one by one
      for (uint64_t i = 0; i < count; i++) {
        int64_t number = 0;
        if (MPI_Recv(&number, 1, MPI_INT64_T, process_number, sending_node,
          world, ignore) != success) {
          return 1;
        }
        // add the number to the array
        array_int_append(sum, number);
      }
      // add the array to the array of sums
      array_of_sums_append(node->array_of_sums, sum);
      free(sum);
    }
  }
  return 0;
}

int get_work_units(int64_t request_id, int64_t process_count,
  int64_t amount_of_elements, int64_t* begin,
  int64_t* end) {
  *begin = start(request_id, amount_of_elements, process_count);
  *end = finish(request_id, amount_of_elements, process_count);
  return 0;
}

int64_t start(int64_t i, int64_t D, int64_t w) {
  // int start_index = i * ( D / w ) + min (i, D mod w)
  int64_t start = i * (D / w) + MIN(i, D % w);
  // return start_index
  return start;
}
// procedure finish(int i, int D, int w):
int64_t finish(int64_t i, int64_t D, int64_t w) {
  // int end_index = start(i + 1, D, w)
  int64_t end = start(i + 1, D, w);
  // return end_index
  return end;
}

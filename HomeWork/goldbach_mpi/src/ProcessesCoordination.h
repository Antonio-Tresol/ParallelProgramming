// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#ifndef PROCESSES_COORDINATION_H
#define PROCESSES_COORDINATION_H
#include <mpi.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "input_output.h"
#include "Array_of_Nodes.h"
#include "Array_int.h"
#include "Array_of_Sums.h"
#include "goldbach.h"
#include "Array_of_coordinates.h"

#define world MPI_COMM_WORLD
#define any_process MPI_ANY_SOURCE
#define any_tag MPI_ANY_TAG
#define ignore MPI_STATUS_IGNORE
#define success MPI_SUCCESS

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
* @brief this procedure computes the goldbach sums of an array of nodes
* but it is done by a single process
* @param A a pointer to the array of nodes of information
* @param mem the shared memory of the threads
* @param max the maximum number to be computed
* @param P a pointer to an array of prime numbers
* @param S a pointer to a boolean array of size max+1
* @param thread_count the number of threads
* @details it assumes that all pointers have been allocated
* @returns int with error code
*/
int compute_goldbach_alone(shared_mem_t* mem, array_of_nodes_t* A, int64_t max,
  array_int_t* P, bool* S, int64_t thread_count);
/**
* @brief this procedure computes the goldbach sums of an array of nodes
* distributed between processes dynamically
* @param A a pointer to the array of nodes of information
* @param mem the shared memory of the threads
* @param max the maximum number to be computed
* @param P a pointer to an array of prime numbers
* @param S a pointer to a boolean array of size max+1
* @param thread_count the number of threads
* @param process_count the number of processes
* @param process_number the number of the process
* @details it assumes that all pointers have been allocated
* @returns int with error code
*/
int compute_goldbach_together(shared_mem_t* mem, array_of_nodes_t* A,
  int64_t max, array_int_t* P, bool* S, int process_number, int process_count,
  int64_t thread_count);
/**
* @brief this procedure sends the base information to the other processes
* @param elements_count the amount of elements in the array of nodes
* @param max the biggest number in all the work units
* @param A the array of nodes
* @param process_count the number of processes
*/
int send_base_info(int64_t elements_count, int64_t max, array_of_nodes_t* A,
  int process_count);
/**
* @brief this procedure receives the base information from the process 0
* @param elements_count the amount of elements in the array of nodes
* @param max the biggest number in all the work units
* @param A the array of nodes
*/
int receive_base_info(int64_t* elements_count,
  int64_t* max, array_of_nodes_t* A);
/**
* @brief this procedure sends the results of the goldbach sums to the process 0
* @param A the array of nodes of information
* @param C the array of cordinates of the blocks of work units
* @param process_number the number of process
* @details it sends only the nodes that were computed by the process using the 
* cordinates.
*/
int send_results(array_of_nodes_t* A, array_coordinates_t* C,
  int process_number);
/**
* @brief this procedure receives the results of the goldbach sums from the other
* processes
* @param A the array of nodes of information
* @param process_count the amount of processes
*/
int receive_results(array_of_nodes_t* A, int64_t process_count);
/**
* @brief this procedure sends a node to a process
* @param node the node to send
*/
int send_node(node_t* node);
/**
* @brief this procedure receives a node from a process
* @param node the node to receive
* @param process_number the process to receive the node from
*/
int receive_node(node_t* node, int process_number);
/**
* @brief returns two indexes to a process to work on based on request id.
* @details request id is independent of the process id, it is a counter that
* increases every time a process requests work. 
* @param request_id the request id
* @param process_count the number of processes
* @param begin the start index
* @param end the end index
* @param amount_of_elements the amount of elements in the array of nodes
* @returns int with error code
*/
int get_work_units(int64_t request_id, int64_t process_count,
  int64_t amount_of_elements, int64_t* begin, int64_t* end);
/**
* @brief finds the start index of the work unit of a thread
* @param i the id of the thread
* @param D the number of work units
* @param w the amount of threads
* @return the start index of the work unit of a thread
*/
int64_t start(int64_t i, int64_t D, int64_t w);
/**
* @brief finds the finish index of the work unit of a thread
* @param i the id of the thread
* @param D the number of work units
* @param w the amount of threads
* @return the finish index of the work unit of a thread
*/
int64_t finish(int64_t i, int64_t D, int64_t w);

#endif

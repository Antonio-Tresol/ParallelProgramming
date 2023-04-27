// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Array_of_Nodes.h>
#include <inttypes.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#define ABS(a) ((a) < (0) ? (a*-1) : (a))
/**
 * @brief reads n numbers from the user and stores them in an array of nodes
 * @param array_of_nodes the array of nodes to store the numbers
 * @return  array of nodes of information (reference/pointer)
 */
int64_t* read_numbers(array_of_nodes_t* array_of_nodes);
/**
 * @brief  prints the goldbach sums
 * @details it prints the number, the number of sums and the sums
 * @param array_of_nodes the array of nodes to print
 */
void print_goldbach_sums(array_of_nodes_t* array_of_nodes);

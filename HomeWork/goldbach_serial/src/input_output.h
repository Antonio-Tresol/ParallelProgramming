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
#include <abs_number.h>
/**
 * @brief reads n numbers from the user and stores them in an array of nodes
 * 
 * @return an array of nodes of information (reference/pointer)
 */
uint64_t* read_numbers(array_of_nodes_t* array_of_nodes);
/**
 * @brief  prints the goldbach sums of a number
 * it prints the number, the number of sums and the sums
 * 
 * @param node is the node of information of the number
 *
 */
void print_goldbach_sums(array_of_nodes_t* array_of_nodes);

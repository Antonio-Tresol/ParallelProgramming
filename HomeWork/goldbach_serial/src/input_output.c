// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <input_output.h>
/*
      while we don't receive a space do
          read int n from user
          add int n in new node of information in A
      end for
end procedure
*/
// procedure read_numbers():
uint64_t* read_numbers(array_of_nodes_t* array_of_nodes) {
  errno = 0;
  int error = 0;
  int64_t number = 0;
  uint64_t* max_number = (uint64_t*)calloc(1, sizeof(uint64_t));
  printf("Enter a number or press control d to stop: ");
  // while we don't receive a signal do
  //  read int n from user
  while (scanf(" %" SCNd64, &number) == 1 && error == 0) {
    if (errno == ERANGE) {
      fprintf(stderr, "Number is too big\n");
      errno = 0;
      error = array_of_nodes_append(array_of_nodes, -1);
      // add int n in new node of information in A
    } else if ( abs_number(number) <= 5 ) {
      fprintf(stderr, "Number is too small\n");
      error = array_of_nodes_append(array_of_nodes, number);
      // add int n in new node of information in A
    } else {
      error = array_of_nodes_append(array_of_nodes, number);
      if (abs_number(number) > *max_number) {
        *max_number = abs_number(number);
      }
    }
  }
  return max_number;
}
void print_goldbach_sums_of_a_node(node_t* node) {
  uint64_t i;
  uint64_t j;
  printf("%" PRId64 ": ", node->number);
  printf("%" PRIu64 " sums: ", node->number_of_sums);
  for ( i = 0; i < node->number_of_sums; i++ ) {
    array_int_t* sum = &node->array_of_sums->sums[i];
    for ( j = 0; j < sum->count; j++ ) {
      printf("%" PRIu64 "", sum->elements[j]);
      if ( j < sum->count - 1 ) {
        printf(" + ");
      }
    }
    if ( i < node->number_of_sums - 1 ) {
      printf(", ");
    }
  }
  printf("\n");
}
/**
 * @brief prints the goldbach sums of a simple node
 * @param node is a pointer to a node
 * to print the goldbach sums of
 */
void print_goldbach_node_simple(node_t* node) {
  printf("%" PRId64 ":", node->number);
  printf(" %" PRIu64 " sums\n", node->number_of_sums);
}
/**
 * @brief prints the goldbach sums of a node with invalid number
 * 
 */
void print_to_big_node() {
  printf("NA: NA\n");
}
/**
 * @brief prints the goldbach sums of a node with invalid number
 *  to small
 * @param node is a pointer to a node
 * to print the number
 */
void print_to_small_node(node_t* node) {
  printf("%" PRId64 "", node->number);
  printf(": NA\n");
}
  /*
procedure print_goldbach_sums(an Array of Nodes A):
   for i := 1 to n do
      if A[i] >= upperLimit then
         print number
         print NA
      end if
      else  
         print A[i].numberm
         print numberOfSums
         for j := 1 to A[i].numberOfSums do
            for k := 1 to m do
               print A[i].arrayOfSums[j][k] 
               print " "
            end for
            print jump line
         end for
         end if
      end else
   end for
end procedure
  */
 /**
 * @brief prints all the goldbach sums of a number for each number in the array
 * of nodes of information
 * @param Array_of_nodes a ptr to the array of nodes of information
 **/
void print_goldbach_sums(array_of_nodes_t* array_of_nodes) {
  uint64_t i;
  printf("\n");
  for ( i = 0; i < array_of_nodes->count; i++ ) {
    node_t* node = &array_of_nodes->elements[i];
    if ( node->number == -1 ) {
      print_to_big_node();
    } else if ( abs_number(node->number) <= 5 ) {
      print_to_small_node(node);
    } else if ( node->number > 0 ) {
      print_goldbach_node_simple(node);
    } else {
      print_goldbach_sums_of_a_node(node);
    }
  }
}

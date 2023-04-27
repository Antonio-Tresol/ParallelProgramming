//Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <assert.h>
#include <inttypes.h>  //inttypes is a library that contains the standard data types of 64-bit integers 
#include <pthread.h>  //pthread is a library that contains the POSIX thread interface
#include <stdint.h> // h is a header file in the C standard library to allow programmers to write more portable code by providing a set of typedefs that specify exact-width integer types,
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //unist is a library that defines miscellaneous symbolic constants and types, 
//and declares miscellaneous functions

/**
 * @brief greets from a thread
 */
void* greet(void* data);  //signature of procedure greet
/**
 * @brief procedure to create threads
 */
int create_threads(uint64_t thread_count);  //signature of procedure create_threads

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {  //main is a procedure that is executed when the program is started, 
//here we will receive the number of threads to be created


  int error = EXIT_SUCCESS;  //here we will store the error code of the program
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);  //sysconf is a function that returns the value of a system configuration parameter
  //in this case we will use the number of processors on the system. Worth noting that we use uint64_t because there we can allocate an incredible amount of threads. 
  //it will reduce the likelihood of a memory overflow in case of a large number of threads.
  if (argc == 2) { //defensive programming, if the number of arguments is not equal to 2, 
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {  //if sscanf reads an interger then that will be the number of threads, 

    } else {  //rainy day, if sscanf does not read an integer then the program will exit with error code 11
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  error = create_threads(thread_count); //call procedure create_threads with thread_count as argument
  return error; //return error code
}  // end procedure


int create_threads(uint64_t thread_count) { //procedure create_threads with thread_count as argument
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t)); //allocate memory for thread_count threads with malloc (threads is a pointer to a chunk of memory)
  if (threads) { //if threads is not null
    for (uint64_t thread_number = 0; thread_number < thread_count
        ; ++thread_number) {  //
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
        , /*arg*/ (void*) thread_number);  //create a new thread in the chunck of memory threads and assign it to the thread_number-th thread
        // and execute the procedure greet with thread_number as argument
      if (error == EXIT_SUCCESS) {
      } else {  //rainy day, if pthread_create fails then the program will exit with error code 21
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }  //join all threads

    free(threads);  //free the memory allocated for threads
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:
void* greet(void* data) {
  // assert(data);
  const uint64_t rank = (uint64_t) data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 "\n", rank);
  return NULL;
}  // end procedure

//Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE comments

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief private data of a thread
 */
typedef struct private_data { //struct private_data is a structure that contains the data of a thread. Threads will access this structure by indexing into the array of structures private_data_array.
  uint64_t thread_number;  // rank
  uint64_t thread_count;
  struct private_data* next; // next is a pointer to the next entry in the array of structures private_data_array
} private_data_t;

/**
 * @brief greeting function for a thread
 */
void* greet(void* data);
int create_threads(uint64_t thread_count);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN); //sysconf is a function that returns the value of a system configuration parameter, in this case we will use the number of processors on the system. 
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  error = create_threads(thread_count);
  return error;
}  // end procedure

int create_threads(uint64_t thread_count) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t));//allocate memory for thread_count threads with calloc (private_data is a pointer to a chunk of memory, that is to say an array of structures private_data_t)
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < thread_count
        ; ++thread_number) {
      private_data[thread_number].thread_number = thread_number; //thread_number is the index of the current thread in the array of structures private_data_array
      private_data[thread_number].thread_count = thread_count; //thread_count is the number of threads in the array of structures private_data_array
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
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
    }

    free(private_data);
    free(threads);
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
  private_data_t* private_data = (private_data_t*) data; //here we cast the void pointer data to a pointer to a structure private_data_t
  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"//PRIu64 is a format specifier for unsigned 64-bit integer
    , (*private_data).thread_number, private_data->thread_count);
  return NULL;
}  // end procedure
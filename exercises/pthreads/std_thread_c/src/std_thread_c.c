//Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE comments

#include <assert.h>
#include <inttypes.h> 
#include <pthread.h>
#include <threads.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //unix library for mesuring time, it a lot more scientific than time
#include <unistd.h>

// thread_shared_data_t
//here we create a structure that will be shared between the threads, it has the amount of threads
typedef struct shared_data {
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t
typedef struct private_data { //this is the structure that will be passed to the thread as private data
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief ...
 */
int greet(void* data);
int create_threads(shared_data_t* shared_data); //this is the function that will create the threads and pass on the shared data

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->thread_count = thread_count;

    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time); //CLOCK_MONOTONIC is the system clock, it is the best choice for measuring time as it ignores discontinuous changes to the system’s setting for absolute calendar time.

    error = create_threads(shared_data);

    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9; //here we counte seconds and nanoseconds, to have accurate nanoseconds we use 1e-9

    printf("Execution time: %.9lfs\n", elapsed_time); //here we print the execution time using %.9lf, it is a double with 9 decimal places

    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    return 12;
  }
  return error;
}  // end procedure


int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  thrd_t* threads = (thrd_t*)
    malloc(shared_data->thread_count * sizeof(thrd_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      // create_thread(greet, thread_number)
      error = thrd_create(&threads[thread_number], greet, &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      thrd_join(threads[thread_number], NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:
int greet(void* data) {
  // assert(data);
  private_data_t* private_data = (private_data_t*) data; // here we cast the data to the private_data_t structure
  shared_data_t* shared_data = private_data->shared_data; // here we get the shared_data_t structure from the private_data_t structure so it is easier to use it

  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , private_data->thread_number, shared_data->thread_count);
  return 1;
}  // end procedure

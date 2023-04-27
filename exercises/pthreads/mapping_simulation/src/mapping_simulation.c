// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
/**
 * @brief shared data between threads
 * @param done_with_mapping is a pointer to a barrier
 * @param work_times is a pointer to an array of work_times
 * @param data_amount is the amount of work_times
 * @param worker_amount is the amount of threads
 * @param dynamic_mapping_counter is a counter for dynamic mapping
 * @param can_access_counter is a mutex for dynamic mapping
 */
typedef struct shared_data {
  pthread_barrier_t* done_with_mapping;
  int* work_times;
  int data_amount;
  int worker_amount;
  int dynamic_mapping_counter;
  pthread_mutex_t can_access_counter;
} shared_data_t;
/**
 * @brief private data for each thread
 * @param thread_num is the number of the thread
 * @param shared_data is a pointer to the shared data
 * 
 */
typedef struct private_data {
  int thread_num;
  shared_data_t* shared_data;
} private_data_t;
/**
 * @brief reads the arguments from the command line
 * 
 * @param argc is the amount of arguments
 * @param argv is an array of arguments
 * @param worker_amount is a pointer to the amount of threads
 */
void read_arguments(int argc, char* argv[], int* worker_amount);
/**
 * @brief reads the work_times from the user
 * 
 * @param number_amount is a pointer to the amount of work_times
 * @return int* an array of work_times
 */
int* read_numbers(int* number_amount);
/**
 * @brief the work function for each thread
 * it will call all the mapping functions for each thread
 * @param data is a pointer to the private private_data of each thread
 * @return void* 
 */
void* work(void* data);
/**
 * @brief simulates the serial computation
 * 
 * @param shared_data is a pointer to the shared data
 * @return int elapsed time
 */
int serial_mapping(shared_data_t* shared_data);
/**
 * @brief simulates the block mapping and fills the result array
 * 
 * @param private_data is the private data of each thread
 *
 */
void block_mapping(private_data_t* private_data, int* result);
/**
 * @brief simulates the cyclic mapping and fills the result array
 * 
 * @param private_data is the private data of each thread
 */
void cyclic_mapping(private_data_t* private_data, int* result);
/**
 * @brief simulates the dynamic mapping and fills the result array
 * 
 * @param private_data is the private data of each thread
 */
void dynamic_mapping(private_data_t* private_data, int* result);
/**
 * @brief simulates the computation of the data
 * 
 * @param private_data is the private data of each thread
 * @param pos is the position of the data
 * @return int elapsed time
 */
int process_data(private_data_t* private_data, const int pos);
/**
 * @brief frees the resources
 * 
 * @param work_times is a pointer to the array of work_times
 */
void free_resources(int* work_times);

#define MIN(a, b)\
  (a < b)? a : b

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // we read the amount of workers from the command line and the work times
  int worker_amount = sysconf(_SC_NPROCESSORS_ONLN);
  read_arguments(argc, argv, &worker_amount);
  int number_amount = 0;
  int* work_times = read_numbers(&number_amount);
  // prepare the shared data between the threads
  pthread_barrier_t done_with_mapping;
  pthread_barrier_init(&done_with_mapping, NULL, worker_amount);
  // prepare memory for the threads
  pthread_t* workers = (pthread_t*) calloc(worker_amount, sizeof(pthread_t));
  // set the shared data
  shared_data_t shared_data;
  shared_data.work_times = work_times;
  shared_data.done_with_mapping = &done_with_mapping;
  shared_data.data_amount = number_amount;
  shared_data.worker_amount = worker_amount;
  shared_data.dynamic_mapping_counter = 0;
  // set the mutex
  pthread_mutex_init(&shared_data.can_access_counter, NULL);
  // set memory for the private data
  private_data_t* private_data = (private_data_t*)
    calloc(worker_amount, sizeof(private_data_t));
  // set the private data and send the threads to work
  for (int worker = 0; worker < worker_amount; worker++) {
    private_data[worker].shared_data = &shared_data;
    private_data[worker].thread_num = worker;
    pthread_create(&workers[worker], NULL,
    work, (void*) &private_data[worker]);
  }
  // prepare memory for the results
  int** results = (int**) calloc(worker_amount, sizeof(int*));

  for (int worker = 0; worker < worker_amount; worker++) {
    pthread_join(workers[worker], (void*)&results[worker]);
  }
  // compute the serial mapping
  int previous = serial_mapping(&shared_data);
  // print the results
  printf("%i\t\t", previous);

  for (int worker = 0; worker < worker_amount; worker++) {
    printf("%i\t\t", worker);
  }

  printf("Duration\tSpeedUp \t\tEfficiency\n");

  for (int mapping = 0; mapping < 3; ++mapping) {
    switch (mapping) {
      case 0:
        printf("Block\t\t");
        break;
      case 1:
        printf("Cyclic\t\t");
        break;
      case 2:
        printf("Dynamic\t\t");
        break;
    }
    int duration = 0;
    for (int worker = 0; worker < worker_amount; worker++) {
      printf("%i\t\t", results[worker][mapping]);
      if (results[worker][mapping] > duration) {
        duration = results[worker][mapping];
      }
    }
    double speedup = previous/duration,
    efficiency = speedup/worker_amount;
    printf("%i\t\t", duration);
    printf("%f\t\t", speedup);
    printf("%f\t\t", efficiency);
    printf("\n");
  }
  // free all allocated memory
  for (int worker = 0; worker < worker_amount; worker++) {
    free(results[worker]);
  }
  free(results);
  free_resources(work_times);
  free(private_data);
  free(workers);
  return error;
}

void read_arguments(int argc, char* argv[], int* worker_amount) {
  if (argc == 2) {
    sscanf(argv[1], "%i", worker_amount);
  }
}

int* read_numbers(int* number_amount) {
  int* work_times = (int*) calloc(20, sizeof(int));
  int capacity = 20;
  int number_amount_local = 0;

  while (scanf("%i", &work_times[number_amount_local]) == 1) {
    number_amount_local++;

    if (number_amount_local == capacity) {
      capacity *= 10;
      int* temp = realloc(work_times, capacity * sizeof(int));
      if (temp == NULL) {
        return NULL;
      } else {
        work_times = temp;
      }
    }
  }
  int* temp = realloc(work_times, number_amount_local * sizeof(int));

  if (temp == NULL) {
    return NULL;
  } else {
    work_times = temp;
  }

  *number_amount = number_amount_local;

  return work_times;
}

int get_start_pos(const int i, const int d, const int w) {
  int base = d/w;
  int minimum = MIN(i, d%w);
  int result = ((i * (base)) + minimum);
  return result;
}

void* work(void* data) {
  private_data_t* private_data = (private_data_t*) data;
  int* results = (int*) calloc(3, sizeof(int));
  // block mapping
  block_mapping(private_data, results);
  // cyclic mapping
  cyclic_mapping(private_data, results);
  // dynamic mapping
  dynamic_mapping(private_data, results);
  return (void*) (size_t)results;
}

int serial_mapping(shared_data_t* shared_data) {
  int result = 0;
  for (int position = 0; position < shared_data->data_amount; ++position) {
    result++;
  }
  return result;
}

void block_mapping(private_data_t* private_data, int* result) {
  int start_pos = get_start_pos(private_data->thread_num,
    private_data->shared_data->data_amount,
    private_data->shared_data->worker_amount);

  int stop_pos = get_start_pos(private_data->thread_num + 1,
    private_data->shared_data->data_amount,
    private_data->shared_data->worker_amount);

  for (int position = start_pos; position < stop_pos; ++position) {
    result[0] += process_data(private_data, position);
  }

  pthread_barrier_wait(private_data->shared_data->done_with_mapping);
}

void cyclic_mapping(private_data_t* private_data, int* result) {
  for (int position = private_data->thread_num;
      position < private_data->shared_data->data_amount;
      position += private_data->shared_data->worker_amount) {
    result[1] += process_data(private_data, position);
  }
  pthread_barrier_wait(private_data->shared_data->done_with_mapping);
}

void dynamic_mapping(private_data_t* private_data, int* result) {
  while (true) {
    pthread_mutex_lock(&private_data->shared_data->can_access_counter);
      int position = private_data->shared_data->dynamic_mapping_counter;
      (private_data->shared_data->dynamic_mapping_counter)++;
    pthread_mutex_unlock(&private_data->shared_data->can_access_counter);
    if (position >= private_data->shared_data->data_amount) {
      break;
    }
    result[2] += process_data(private_data, position);
  }
}

int process_data(private_data_t* private_data, const int pos) {
  int time_taken = private_data->shared_data->work_times[pos];
  usleep(time_taken);
  return time_taken;
}

void free_resources(int* work_times) {
  if (work_times != NULL) {
    free(work_times);
  }
}

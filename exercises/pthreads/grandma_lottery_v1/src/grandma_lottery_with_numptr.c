// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * @brief procedure that calculates a random_number number and returns its
 * memory address. To allocate this number on dynamic memory we use malloc.
 * to generate random_number numbers here we use rand_r, it is a function that
 * is a function from the  unix
 */
void* random_number(void* data);

int main(void) {
  //  we create two threads one for each grand son
  pthread_t thread_grandson_1;
  pthread_t thread_grandson_2;
  // a semi-random parameter.
  //  then we create error for each thread
  int error_1;
  int error_2;
  // we create a seed for each thread
  uint32_t seed = time(NULL);
  //  we start the threads to do the random_number number and we pass a seed as
  //  args for the function
  error_1 = pthread_create(&thread_grandson_1, NULL, random_number, &seed);
  error_2 = pthread_create(&thread_grandson_2, NULL, random_number, &seed);
  //  we retrieve the random_number number from the threads
  if (error_1 == EXIT_SUCCESS && error_2 == EXIT_SUCCESS) {
    int64_t* random_number_1;
    int64_t* random_number_2;
    // void pointer cast just to eliminate warning, works perfectly without it
    pthread_join(thread_grandson_1, (void*)&random_number_1);
    pthread_join(thread_grandson_2, (void*)&random_number_2);
    //  we print the random_number number
    printf("lottery number 1: %ld\n", *random_number_1);
    printf("lottery number 2: %ld\n", *random_number_2);
    free(random_number_1);
    free(random_number_2);
  } else {
    printf("error: grandsons were not able to go buy lottery number");
  }
}

void* random_number(void* seed) {
  int64_t* random_number_produced = (int64_t*)calloc(1, sizeof(int64_t));
  uint32_t my_seed = *((uint32_t*)seed) ^ pthread_self();
  if (random_number_produced) {  // if we have allocated memory for the
                                 //  random_number number successfully
    //  generate random_number number between 0 and 99 using rand_r
    *random_number_produced = rand_r(&my_seed) % 101;
    return random_number_produced;
  } else {
    fprintf(stderr, "Error: could not allocate random_number number\n");
    return NULL;
  }
}
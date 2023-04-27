// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
/**
 * @struct threads shared memory
 * also represents the pinata
 * @var life points is the number of hits needed to break the pinata
 * @var base life points is the number of hits needed to break the pinata
 * @var a mutex to protect the pinata
 */
typedef struct {
  int pinata_life_points;
  pthread_mutex_t can_attack_pinata;
} birthday_pinata_t;
/**
 * @struct threads private memory
 * @var id is the thread number
 * @var a pointer to the shared memory
 * @var boolean to know if the thread did break the pinata
 * @var pinata base life points to print the number of hits needed to break it
 *
 */
typedef struct {
  int id;  // thread id
  birthday_pinata_t* birthday_pinata;
  bool did_I_break_the_pinata;
  int count_of_attacks;
  int pinata_base_life_points;
} party_thread_private_memory_t;
/**
 * @brief initialize the party and runs the threads, then joins them
 * @param pinata is a pointer to the shared memory
 * @param private memory is a pointer to the private memory of the threads
 * @param thread_count is the number of threads to create
 * @param pinata_life_points is the number of hits needed to break the pinata
 */
int start_party_of_threads(int thread_count,
  party_thread_private_memory_t* private_memory,
    birthday_pinata_t* pinata, int pinata_base_life_points);
/**
 * @brief attacks the pinata
 * 
 * @param private_memory of the thread
 * @return void* 
 */
void* attack_pinata(void* private_memory);
// int with args
int main(int argc, char* argv[]) {
  // base thead count is the number of processors in the system
  int thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  // declare pointer to birthday pinata in dynamic memory using calloc and
  // cast it to birthday_pinata_t
  birthday_pinata_t* pinata = (birthday_pinata_t*) calloc(
    1, sizeof(birthday_pinata_t));
  if ( pinata == NULL ) {
    fprintf(stderr, "Error: calloc failed\n");
    return EXIT_FAILURE;
  }
  // check if the user provided the number of threads
  if ( argc > 2 ) {
    // read the number of threads
    thread_count = atoi(argv[2]);
    // read the number of hits needed to break the pinata
    pinata->pinata_life_points = atoi(argv[1]);
  } else if ( argc > 1 ) {
    // if the user did not provide the number of threads
    // read the number of hits needed to break the pinata
    pinata->pinata_life_points = atoi(argv[1]);
  } else {
    // if the user did not provide the number of threads
    // print an error message and exit
    fprintf(stderr, "invalid number of arguments\n");
    return EXIT_FAILURE;
  }
  party_thread_private_memory_t* party_thread_private_memory =
    (party_thread_private_memory_t*) calloc(
    thread_count, sizeof(party_thread_private_memory_t));
    int error = pthread_mutex_init(&pinata->can_attack_pinata, NULL);
    if ( error != 0 ) {
      fprintf(stderr, "Error: pthread_mutex_init failed\n");
      return EXIT_FAILURE;
    }
  // start the party
  error = start_party_of_threads(thread_count,
    party_thread_private_memory, pinata, pinata->pinata_life_points);
  if ( error != 0 ) {
    fprintf(stderr, "Error: start_party_of_threads failed\n");
    return EXIT_FAILURE;
  }
  // free the memory
  free(party_thread_private_memory);
  free(pinata);
  return EXIT_SUCCESS;
  // initialize the mutex
  }
  int start_party_of_threads(int thread_count,
    party_thread_private_memory_t* private_memory,
    birthday_pinata_t* pinata, int pinata_base_life_points) {
    pthread_t* threads = (pthread_t*) calloc(thread_count, sizeof(pthread_t));
    if ( threads == NULL ) {
      fprintf(stderr, "Error: calloc failed\n");
      return EXIT_FAILURE;
    }
    for (int i = 0; i < thread_count; i++) {
      private_memory[i].id = i;
      private_memory[i].birthday_pinata = pinata;
      private_memory[i].did_I_break_the_pinata = false;
      private_memory[i].count_of_attacks = 0;
      private_memory[i].pinata_base_life_points = pinata_base_life_points;
      int error = pthread_create(&threads[i], NULL,
        attack_pinata, &private_memory[i]);
      if ( error != 0 ) {
        fprintf(stderr, "Error: pthread_create failed\n");
        return EXIT_FAILURE;
      }
    }
    for (int i = 0; i < thread_count; i++) {
      int error = pthread_join(threads[i], NULL);
      if ( error != 0 ) {
        fprintf(stderr, "Error: pthread_join failed\n");
        return EXIT_FAILURE;
      }
    }
    free(threads);
    return EXIT_SUCCESS;
}
void* attack_pinata(void* private_memory) {
  party_thread_private_memory_t* party_thread_private_memory =
    (party_thread_private_memory_t*) private_memory;
  birthday_pinata_t* pinata = party_thread_private_memory->birthday_pinata;
  // lock the mutex
  while (true) {
    pthread_mutex_lock(&pinata->can_attack_pinata);
    // check if the pinata is still alive
    if ( pinata->pinata_life_points > 0 ) {
      // if the pinata is still alive
      // attack the pinata
      pinata->pinata_life_points--;
      party_thread_private_memory->count_of_attacks++;
      // check if the pinata is still alive
      if ( pinata->pinata_life_points == 0 ) {
        // if the pinata is dead
        // set the boolean to true
        party_thread_private_memory->did_I_break_the_pinata = true;
      }
      pthread_mutex_unlock(&pinata->can_attack_pinata);
    } else {
      pthread_mutex_unlock(&pinata->can_attack_pinata);
      if ( party_thread_private_memory->did_I_break_the_pinata == true ) {
        printf("Thread %d / %d: %d hits", party_thread_private_memory->id,
          party_thread_private_memory->pinata_base_life_points,
          party_thread_private_memory->count_of_attacks);
        printf(", I broke the pinata\n");
      } else {
        printf("Thread %d / %d: %d hits\n", party_thread_private_memory->id,
          party_thread_private_memory->pinata_base_life_points,
          party_thread_private_memory->count_of_attacks);
      }
      break;
    }
  }
  return NULL;
}



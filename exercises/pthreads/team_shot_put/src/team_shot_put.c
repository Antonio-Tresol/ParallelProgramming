// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
// receive an int from the console as args, if the number is not
// odd print in standard error "the number is not odd" and exit with code 1
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#define MAX_DISTANCE 25
#define MAX_TEAMS 2
#define MAX_SHOTS 3

pthread_t* create_team(int num_athletes);
void* make_shots_Team_1(void* data);
void* make_shots_Team_2(void* data);
// we recieve the number of athletes from the console, it must be odd
int main(int argc, char *argv[]) {
  int64_t number_of_athletes = 0;
  if ( argc == 2 ) {
    if (sscanf(argv[1], "%" SCNu64, &number_of_athletes) == 1) {
      if ( number_of_athletes % 2 == 0 ) {
        fprintf(stderr, "the number of athletes is not odd");
        exit(1);
      }
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }
  if ( number_of_athletes % 2 == 0 ) {
    fprintf(stderr, "the number of athletes is not odd");
    exit(1);
  }
  // we create two teams
  pthread_t* team_one = create_team(number_of_athletes);
  pthread_t* team_two = create_team(number_of_athletes);
  int error_team_one_in = EXIT_SUCCESS;
  int error_team_two_in = EXIT_SUCCESS;
  int error_team_one_out = EXIT_SUCCESS;
  int error_team_two_out = EXIT_SUCCESS;
  int64_t points_team_one = 0;
  int64_t points_team_two = 0;
  for (int64_t i = 0; i < number_of_athletes; i++) {
    // make shots for athlete team one
    error_team_one_in = pthread_create(&team_one[i], NULL, make_shots_Team_1,
      (void*) i);
    if ( error_team_one_in != 0 ) {
        fprintf(stderr,
        "Error: Athlete in team 1 couldn't make its shots() failed: %d",
         error_team_one_in);
       exit(error_team_one_in);
    }
    void* best_shot_one = 0;
    // get best shot with join
    error_team_one_out = pthread_join(team_one[i], &best_shot_one);
    if ( error_team_one_out != 0 ) {
        fprintf(stderr,
        "Error: Athlete in team 1 couldn't make its shots() failed: %d",
         error_team_one_out);
       exit(error_team_one_out);
    }
    // cast void recieve to ptr to double
    double best_shot_one_double = *((double*) best_shot_one);
    // Now make shots for athlete team 2
    error_team_two_in = pthread_create(&team_two[i], NULL, make_shots_Team_2,
      (void*) i);
    if ( error_team_two_in != 0 ) {
        fprintf(stderr,
        "Error: Athlete in team 2 couldn't make its shots() failed: %d",
        error_team_two_in);
     exit(error_team_two_in);
    }
    void* best_shot_two = 0;
    error_team_two_out = pthread_join(team_two[i], &best_shot_two);
    if ( error_team_two_out != 0 ) {
        fprintf(stderr,
        "Error: Athlete in team 2 couldn't join() failed: %d",
        error_team_one_out);
        exit(error_team_one_out);
    }
    double best_shot_two_double = *((double*) best_shot_two);
    // compare the best shots of each team
    if ( best_shot_one_double > best_shot_two_double ) {
      points_team_one++;
    } else if ( best_shot_one_double < best_shot_two_double ) {
      points_team_two++;
    }
    free(best_shot_one);
    free(best_shot_two);
  }
    // print the winner
  printf("result %" PRId64, points_team_one);
  printf(":%" PRId64, points_team_two);
  if ( points_team_one > points_team_two ) {
    printf(" Team 1 wins\n");
  } else if ( points_team_one < points_team_two ) {
    printf(" Team 2 wins\n");
  } else {
    printf(" Draw\n");
  }
    free(team_one);
    free(team_two);
    return 0;
}
/**
 * @brief Create a team of threads
 * 
 * @param numberOfAthlets is the number of threads
 * @return pthread_t* with the threads representing the athletes
 */
pthread_t* create_team(int num_athletes) {
  // we create a pointer thread for each athlete
  pthread_t* athletes = (pthread_t*) calloc(num_athletes, sizeof(pthread_t));
  if (athletes) {
    return athletes;
  } else {
    fprintf(stderr, "Error creating the team");
    exit(EXIT_FAILURE);
  }
}
/**
 * @brief makes 3 shots per athlete and prints the best one
 * 
 * @param data pass the id of the athlete
 * @return void* 
 */
void* make_shots_Team_1(void* data) {
  int64_t id = (int64_t) data;
  unsigned int seed_1 = (time(NULL) & 0xFFFF0FFF) | (id + 1);
  // we create a random number of shots for each athlete
  double* shots = (double*) calloc(MAX_SHOTS, sizeof(double));
  for (int i = 0; i < MAX_SHOTS; i++) {
  // we create a random distance for each shot
    shots[i] = (double) (rand_r(&seed_1) % MAX_DISTANCE+1);
  }
  // we find the best shot
  double* best_shot = (double*) calloc(1, sizeof(double));
  *best_shot = shots[0];
  for (int i = 1; i < MAX_SHOTS; i++) {
    if (shots[i] > *best_shot) {
      *best_shot = shots[i];
    }
  }
    printf("1:%" PRId64, id + 1);
    printf(":%.3fm\n", *best_shot);
    printf("\n");
    free(shots);
    return (void*) best_shot;
}
/**
 * @brief makes 3 shots per athlete and prints the best one
 * 
 * @param data pass the id of the athlete
 * @return void* 
 */
void* make_shots_Team_2(void* data) {
  int64_t id = (int64_t) data;
  unsigned int seed_2 = (time(NULL) & 0xFFFFFFF0) | (id + 1);
  // we create a random number of shots for each athlete
  double* shots = (double*) calloc(MAX_SHOTS, sizeof(double));
  for (int i = 0; i < MAX_SHOTS; i++) {
  // we create a random distance for each shot
    shots[i] = (double) (rand_r(&seed_2) % MAX_DISTANCE+1);
  }
  // we find the best shot
  double* best_shot = (double*) calloc(1, sizeof(double));
  *best_shot = shots[0];
  for (int i = 1; i < MAX_SHOTS; i++) {
    if (shots[i] > *best_shot) {
      *best_shot = shots[i];
    }
  }
    printf("2:%" PRId64, id + 1);
    printf(":%.3fm\n", *best_shot);
    printf("\n");
    free(shots);
    return (void*) best_shot;
}


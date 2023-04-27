// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef SIMULATION_H
#define SIMULATION_H
/**
 * @brief simulation data
 * 
 */
typedef struct simulation simulation_t;  // opaque record
/**
 * @brief initializes a simulation
 * 
 * @param simulation simulation to initialize
 * @param unit_count number of units to produce
 * @param producer_count number of producers
 * @param consumer_count number of consumers
 * @param producer_min_delay minimum delay of a producer
 * @param producer_max_delay maximum delay of a producer
 * @param consumer_min_delay minimum delay of a consumer
 * @param consumer_max_delay maximum delay of a consumer
 * @return int 
 */
simulation_t* simulation_create();
int simulation_run(simulation_t* simulation, int argc, char* argv[]);
/**
 * @brief destroys a simulation
 * 
 * @param simulation simulation to destroy
 */
void simulation_destroy(simulation_t* simulation);

#endif  // SIMULATION_H

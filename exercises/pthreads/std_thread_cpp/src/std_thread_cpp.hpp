// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE comments
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
/**
 * @brief struct to store the shared data of a team of threads
 * @details has thead count
 */
struct shared_data_t;
/**
 * @brief struct to store the private data of a thread
 * @details has thead number and a pointer to the shared data
 */
struct private_data_t;
/**
 * @brief Create threads
 * @param shared_data of all threads
 */
void create_threads(shared_data_t* shared_data);
/**
 * @brief greets from a std::thread
 * @param private_data is a pointer to a private_data_t struct
 * belonging to the std::thread
 */
void greet(private_data_t* private_data);



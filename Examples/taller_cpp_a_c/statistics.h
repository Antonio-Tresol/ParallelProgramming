#ifndef STATISTICS_H // ifndef STATISTICS_H
#define STATISTICS_H // define STATISTICS_H
#include <assert.h>
#include <stdbool.h>
#include "array_double.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 
 * 
 */
typedef struct statistics statistics_t;

/**
 * @brief Constructor
 * @details Initializes a statistics_t structure.
 * @code
  statistics_t statistics;
  statistics_init(&statistics);
  int error = statistics_run(&statistics, argc, argv)
  statistics_destroy(&statistics);
 * @endcode
 * @param statistics
 * @return
 * @remark
*/
statistics_t* statistics_create(void);

int statistics_run(statistics_t* statistics, int argc, char* argv[]);

void statistics_destroy(statistics_t* statistics);

#endif // STATISTICS_H

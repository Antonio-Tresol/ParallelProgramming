// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }
  // here we make a parallel for loop with the number of threads. 
  // internally we'll use this threads we created to do other things
  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(iteration_count, std::cout)
  { //start of parallel region
    #pragma omp for // this is a for loop that is executed by the threads
    // for without the keyword that specifies that it is a parallel for loop
    // will asume that the threads are created outside the loop
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
      #pragma omp critical(stdout)
      std::cout << "stage 1: " << omp_get_thread_num() << '/'
        << omp_get_num_threads() << ": iteration " << iteration << '/'
        << iteration_count << std::endl;
    } // end of for loop
    // implicit barrier!  #pragma omp barrier
    /// after each for in omp there is a barrier, this means that the threads
    /// will wait until all the threads finish the for loop before executing the
  
    // the single directive is used to specify that only one of the threads
    // will execute the code inside the single directive
    // and the other threads will wait until the thread that is executing the
    // single directive finishes
    #pragma omp single // only one thread executes this
    std::cout << /*omp_get_thread_num() <<*/ std::endl;
   // implicit barrier!  #pragma omp barrier 

    #pragma omp for // this is a parallel for loop
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
      #pragma omp critical(stdout)
      std::cout << "stage 2: " << omp_get_thread_num() << '/'
        << omp_get_num_threads() << ": iteration " << iteration << '/'
        << iteration_count << std::endl;
    } //end of parallel region
    // barrier! #pragma omp barrier
    #pragma omp single
    std::cout << /*omp_get_thread_num() <<*/ std::endl;
    // implicit barrier! #pragma omp barrier

    #pragma omp for // this is a parallel for loop
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
      #pragma omp critical(stdout)
      std::cout << "stage 3: " << omp_get_thread_num() << '/'
        << omp_get_num_threads() << ": iteration " << iteration << '/'
        << iteration_count << std::endl;
    } // end of parallel for loop
    //implicit barrier! #pragma omp barrier
  } //end of parallel region
}
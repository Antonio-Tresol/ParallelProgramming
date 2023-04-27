// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <iostream>

int main() {
  // preprocessor directive telling the main thread to create a n threads to 
  // execute the following code
  // n is the number of threads that can be created in the system running the
  // program
  #pragma omp parallel
  {
    // omp critical is like a mutex
    #pragma omp critical
    std::cout << "Hello from secondary thread" << std::endl;
  }  //  after the paralell block the main thread will join the secondary
  //  threads
}
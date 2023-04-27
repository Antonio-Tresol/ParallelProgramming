// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <omp.h>
#include <iostream>
#include <algorithm>
#include "UniformRandom.hpp"
/**
 * @brief Generates pseudorandom numbers using a uniform distribution
 * @param amount is the amount of numbers to generate
 * @param numbers is the array where the numbers will be stored
 */
void genRandNumbers(int amount, double* numbers);
/**
 * @brief uses the odd-even sort algorithm to sort an array of numbers
 * @param n is the size of the array
 * @param arr is the array to be sorted
 * @param threadAmount is the amount of threads to use
 * @details the odd-even sort algorithm when run serially is not very efficient
 * it is O(n^2) and it is not stable. its strength is that, when run in parallel,
 * it is very efficient.
 */
void oddEvenSortFor(size_t n, double* arr, int threadAmount);
void oddEvenSortTwoFor(size_t n, double* arr, int threadCount);
void oddEvenSortForSC(size_t n, double* arr, int threadAmount);
void oddEvenSortTwoForSC(size_t n, double* arr, int threadCount);
/**
 * @brief swaps two numbers
 * @param a is the first number
 * @param b is the second number
 */
void printNumbs(double* numbers, int numberAmount);
/**
 * @brief checks if an array of numbers is in order
 * @param numbers is the array to be checked
 * @param numberAmount is the size of the array
 * @return true if the array is in order, false otherwise
 */
bool isOrdered(double* numbers, int numberAmount);
/**
 * @brief Main to test the odd-even sort algorithm in parallel
 * receives the opcion  as first argument and thread count as second
 * @param argc 
 * @param argv 
 * @return int 
 * @details opc 1 is for odd-even sort with parallel for, creates a thread for
 * for each for of the algorithm(has critical section), opc 2 is for odd-even sort with parallel two for
 * it creates only one team for the two for loops in the algorithm, opc 3 is odd-even
 * sort with parallel for and without the critical section, 
 * opc 4 is for odd-even sort with parallel two for and without the critical section
 */
int main(int argc, char* argv[]) {
  int numberAmount = 100000;
  int opc = 1;
  int threadCount = 1;
  if (argc >= 2) {
    opc = atoi(argv[1]);
  }
  if (argc >= 3) {
    threadCount = atoi(argv[2]);
  } else {
    threadCount = omp_get_max_threads();
  }
  std :: cout << "Number of threads: " << threadCount << std :: endl;
  std :: cout << "OPC: " << opc << std :: endl;
  double* numbers = new double[numberAmount];
  genRandNumbers(numberAmount, numbers);
  //printNumbs(numbers, numberAmount);
  switch (opc) {
    case 1:
      std::cout << "oddEvenSortFor" << std::endl;
      oddEvenSortFor(numberAmount, numbers, threadCount);
      break;
    case 2:
      std::cout << "oddEvenSortTwoFor" << std::endl;
      oddEvenSortTwoFor(numberAmount, numbers, threadCount);
      break;
    case 3:
      std::cout << "oddEvenSortForSC" << std::endl;
      oddEvenSortForSC(numberAmount, numbers, threadCount);
      break;
    case 4:
      std::cout << "oddEvenSortTwoForSC" << std::endl;
      oddEvenSortTwoForSC(numberAmount, numbers, threadCount);
      break;
    default:
      oddEvenSortFor(numberAmount, numbers, threadCount);
      break;
  }
  std::cout << "ordered" << std::endl;
  //printNumbs(numbers, numberAmount);
  std::cout <<
  (isOrdered(numbers, numberAmount)? "All in order" : "Not in order")
  << std::endl;
  delete[] numbers;
  return 0;
}

void genRandNumbers(int amount, double* numbers) {
  UniformRandom<float> uniformRandom;
  for (int num = 0; num < amount; num++) {
    numbers[num] = uniformRandom.between(0, 10000000);
  }
}

void oddEvenSortFor(size_t n, double* arr, int threadCount) {
  for (int phase = 0; phase < n; ++phase) {
    if (phase % 2 == 0) {
      #pragma omp parallel for num_threads(threadCount) \
      default(none) shared(arr, n)
      for (int i = 1; i < n; i += 2) {
        if (arr[i - 1] > arr[i]) {
          #pragma omp critical(canAccessArr)
          std::swap(arr[i - 1], arr[i]);
        }
      }
    } else {
      #pragma omp parallel for num_threads(threadCount) \
      default(none) shared(arr, n)
      for (int i = 1; i < n - 1; i += 2) {
        if (arr[i] > arr[i + 1]) {
          #pragma omp critical(canAccessArr)
          std::swap(arr[i], arr[i + 1]);
        }
      }
    }
  }
}

void oddEvenSortTwoFor(size_t n, double* arr, int threadCount) {
  #pragma omp parallel num_threads(threadCount)default(none) shared(arr, n)
  {
    for (int phase = 0; phase < n; ++phase) {
      if (phase % 2 == 0) {
        #pragma omp for
        for (int i = 1; i < n; i += 2) {
          if (arr[i - 1] > arr[i]) {
            #pragma omp critical(canAccessArr)
            std::swap(arr[i - 1], arr[i]);
          }
        }
      } else {
        #pragma omp for
        for (int i = 1; i < n - 1; i += 2) {
          if (arr[i] > arr[i + 1]) {
            #pragma omp critical(canAccessArr)
            std::swap(arr[i], arr[i + 1]);
          }
        }
      }
    }
  }
}

void oddEvenSortForSC(size_t n, double* arr, int threadCount) {
  for (int phase = 0; phase < n; ++phase) {
    if (phase % 2 == 0) {
      #pragma omp parallel for num_threads(threadCount) \
      default(none) shared(arr, n)
      for (int i = 1; i < n; i += 2) {
        if (arr[i - 1] > arr[i]) {
          std::swap(arr[i - 1], arr[i]);
        }
      }
    } else {
      #pragma omp parallel for num_threads(threadCount) \
      default(none) shared(arr, n)
      for (int i = 1; i < n - 1; i += 2) {
        if (arr[i] > arr[i + 1]) {
          std::swap(arr[i], arr[i + 1]);
        }
      }
    }
  }
}

void oddEvenSortTwoForSC(size_t n, double* arr, int threadCount) {
  #pragma omp parallel num_threads(threadCount)default(none) shared(arr, n)
  {
    for (int phase = 0; phase < n; ++phase) {
      if (phase % 2 == 0) {
        #pragma omp for
        for (int i = 1; i < n; i += 2) {
          if (arr[i - 1] > arr[i]) {
            std::swap(arr[i - 1], arr[i]);
          }
        }
      } else {
        #pragma omp for
        for (int i = 1; i < n - 1; i += 2) {
          if (arr[i] > arr[i + 1]) {
            std::swap(arr[i], arr[i + 1]);
          }
        }
      }
    }
  }
}
void printNumbs(double* numbers, int numberAmount) {
  std::cout << "numbers:" << std::endl;
  for (int num = 0; num < numberAmount; ++num) {
    std::cout << numbers[num] << std::endl;
  }
}

bool isOrdered(double* numbers, int numberAmount) {
  for (int number = 1; number < numberAmount; number++) {
    if (numbers[number - 1] > numbers[number]) {
      return false;
    }
  }

  return true;
}

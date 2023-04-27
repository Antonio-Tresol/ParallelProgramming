// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <iostream>
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
 * @details the odd-even sort algorithm when run serially is not very efficient
 * it is O(n^2) and it is not stable. its strength is that, when run in parallel,
 * it is very efficient.
 */
void serialOddEvenSort(size_t n, double* arr);
/**
 * @brief swaps two numbers
 * @param a is the first number
 * @param b is the second number
 */
void swap(double* a, double* b);
/**
 * @brief prints an array of numbers
 * @param numbers is the array to be printed
 * @param number_amount is the size of the array
 */
void printNumbs(double* numbers, int number_amount);

int main(int argc, char* argv[]) {
  int number_amount = 0;
  if (argc >= 2) {
    number_amount = atoi(argv[1]);
    double* numbers = new double[number_amount];
    genRandNumbers(number_amount, numbers);
    printNumbs(numbers, number_amount);
    serialOddEvenSort(number_amount, numbers);
    std::cout << "ordered" << std::endl;
    printNumbs(numbers, number_amount);
    delete[] numbers;
  }
  return 0;
}

void genRandNumbers(int amount, double* numbers) {
  size_t upper_limit = 99;
  UniformRandom<float> uniformRandom;
  uniformRandom.between(0, 99);
  for (int num = 0; num < amount; num++) {
    numbers[num] = uniformRandom.between(0, 100);
  }
}

void serialOddEvenSort(size_t n, double* arr) {
  bool isSorted = false;
  while (!isSorted) {
    isSorted = true;
    for (int i = 1; i <= n - 2; i = i + 2) {
      if (arr[i] > arr[i + 1]) {
        swap(&arr[i], &arr[i + 1]);
        isSorted = false;
      }
    }
    for (int i = 0; i <= n - 2; i = i + 2) {
      if (arr[i] > arr[i + 1]) {
        swap(&arr[i], &arr[i + 1]);
        isSorted = false;
      }
    }
  }
}
void swap(double* a, double* b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}
void printNumbs(double* numbers, int number_amount) {
  std::cout << "numbers:" << std::endl;
  for (int num = 0; num < number_amount; ++num) {
    std::cout << numbers[num] << std::endl;
  }
}

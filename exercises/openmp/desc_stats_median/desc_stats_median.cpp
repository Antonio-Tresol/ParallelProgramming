// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <cmath>
#include <omp.h>

int main() {
  std::vector<double> numbers;
  int thread_count = omp_get_max_threads();
  std::cout << "Enter the numbers, ended Ctrl+D:" << std::endl;
  double number;
  while (std::cin >> number) {
    numbers.push_back(number);
  }
  if (numbers.size() == 0) {
    std::cout << "No numbers entered." << std::endl;
    return 0;
  }
  double minimum = numbers[0];
  double maximum = numbers[0];
  double average = 0;
  double stdDev = 0;
  double median = 0;
  std::sort(numbers.begin(), numbers.end());
#pragma omp parallel for num_threads(thread_count)default(none) shared(numbers)\
reduction(min:minimum) reduction(max:maximum) reduction(+:average)\
reduction(+:stdDev) 
  for (auto i = 0; i < numbers.size(); i++) {
    if (numbers[i] < minimum) {
        minimum = numbers[i];
    }
    if (numbers[i] > maximum) {
        maximum = numbers[i];
    }
    average += numbers[i]/numbers.size();
    stdDev += (numbers[i] - average) * (numbers[i] - average);
  }

  stdDev = std::sqrt(stdDev/(numbers.size()-1));
  std::cout << "Minimum: " << minimum << std::endl;
  std::cout << "Maximum: " << maximum << std::endl;
  std::cout << "Median: " << numbers[(numbers.size()/2) - 1] << std::endl;
  std::cout << "Average: " << average << std::endl;
  std::cout << "Standard Deviation: " << stdDev << std::endl;
  return 0;
}

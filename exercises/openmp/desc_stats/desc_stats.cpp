// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

int main() {
  std::vector<double> numbers;
  std::cout << "Enter the numbers, ended Ctrl+D:" << std::endl;
  int thread_count = omp_get_max_threads();
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
  double standardDeviation = 0;

#pragma omp parallel for num_threads(thread_count)default(none) shared(numbers) \
 reduction(min:minimum) reduction(max:maximum) reduction(+:average) \
 reduction(+:standardDeviation) 
   //  parallel region
  for (auto i = 0; i < numbers.size(); i++) {
    if (numbers[i] < minimum) {
      minimum = numbers[i];
    }
    if (numbers[i] > maximum) {
      maximum = numbers[i];
    }
    average += numbers[i]/numbers.size();
    standardDeviation += (numbers[i] - average) *
        (numbers[i] - average);
  }
// end parallel region
standardDeviation = std::sqrt(standardDeviation/(numbers.size()-1));

std::cout << "Minimum: " << minimum << std::endl;
std::cout << "Maximum: " << maximum << std::endl;
std::cout << "Average: " << average << std::endl;
std::cout << "Standard Deviation: " << standardDeviation << std::endl;
return 0;
}

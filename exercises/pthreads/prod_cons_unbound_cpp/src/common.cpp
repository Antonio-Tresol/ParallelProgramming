// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <random>
double random_between(double min, double max) {
  unsigned int seed = (min + min)/2;
  double ans = rand_r(&seed);
  while (ans > max && ans > min) {
    ans /= 2;
  }
  return ans;
}

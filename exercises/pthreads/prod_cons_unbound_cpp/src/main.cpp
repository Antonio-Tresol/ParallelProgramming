// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <cstdlib>

#include "simulation.hpp"

int main(int argc, char* argv[]) {
  simulation* simulation = new class simulation();

  simulation->run(argc, argv);

  delete(simulation);
}

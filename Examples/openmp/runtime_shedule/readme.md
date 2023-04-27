Cree una versión reducida de Ejemplo 37 que escoja el tipo de mapeo (scheduling) en tiempo de ejecución con la variable ambiente OMP_SCHEDULE. Por ejemplo:

$ OMP_SCHEDULE=static bin/runtime_schedule 3 10
runtime    0 0 0 0 1 1 1 2 2 2

$ OMP_SCHEDULE=static,1 bin/runtime_schedule 3 10
runtime    0 1 2 0 1 2 0 1 2 0
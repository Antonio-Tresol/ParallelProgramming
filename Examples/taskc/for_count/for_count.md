# for_count

¿Cuál es el valor más grande que la variable compartida count podría llegar a obtener? ¿En qué rutas de ejecución se alcanza este valor?
  10000, en cualquier execution path serializado
  serial con 1 cpu, por ejemplo

¿Cuál es el menor valor que la variable compartida count podría llegar a obtener? ¿En qué rutas de ejecución se alcanza este valor?
  100, cuando todos los hilos trabajen paralelamente.
  100 cpus a causa de la condicion de carrera
  tambien, un hilo desfasado, le cae encima al trabajo de todos los hilos. 
  
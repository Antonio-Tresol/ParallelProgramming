Modifique la simulación de red para recibir en un argumento de línea de comandos la capacidad máxima de paquetes que cabe en cada cola de la simulación. Modifique la plantilla Queue para que represente un buffer acotado. Sugerencia, agregue un semárofo canProduce que se inicialice en esta capacidad indicada por el usuario en el constructor de la cola. Si este número no se provee, suponga el número más grande que cabe en una variable unsigned.

Corra su simulación de red con dieferentes límites en las colas ¿Se afecta el tiempo de ejecución?
Respuesta:
- Segun las ejecuciones realizadas: afecta el tiempo de ejecucion de que tan grande sea el buffer. Entre mas grande sea el buffer, parece que el tiempo de ejecucion se acerca mas al de un buffer infinito. 
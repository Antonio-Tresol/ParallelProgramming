Modifique la solución de la Ejemplo 48 (send_recv_urd) para que:

Los procesos lean un arreglo de números en la entrada estándar con los cuales trabajar. Luego cada proceso imprime los números que leyó en la salida estándar.

Haga las correcciones para que todos los procesos tengan y puedan efectivamente imprimir el arreglo.

Los procesos reporten en la salida estándar la cantidad de segundos en que tarda su ejecución.
MPI permite que sólo el proceso 0 lea de la entrada estándar. Más específicamente, la entrada la lee el comando mpiexec, la captura y la envía al proceso 0. Los demás procesos no reciben la entrada, por tanto si intentan leer, se quedarán bloqueados perennemente. Si un proceso debe leer de la entrada estándar, el proceso 0 tendrá que usar comunicación para enviarla a los demás procesos.

Para calcular la duración en segundos del tiempo en la pared use la función MPI_Wtime. La medición del tiempo en los sistemas distribuidos es un problema sumamente complejo, dado que los procesos pueden ejecutarse en máquinas distintas, y por consiguiente, con diferentes relojes no perfectamente sincronizados entre sí. La función MPI_Wtime() reporta un tiempo transcurrido en segundos que un proceso puede usar como referencia para sí mismo, pero no es válido para otros procesos.
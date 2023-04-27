Haga que los threads saluden siempre en orden. Es decir, si se crean w threads, la salida sea siempre en orden

Hello from thread 0 of w
Hello from thread 1 of w
Hello from thread 2 of w
...
Hello from thread w of w
Utilice espera activa como mecanismo de sincronización (control de concurrencia).

Ejecute varias veces su solución de hello_order_busywait con la cantidad máxima de threads que el su sistema operativo le permite. ¿Es consistente la duración de su solución con espera activa?

Describa el comportamiento del programa y explique la razón de este comportamiento en el archivo readme.md dentro de su carpeta hello_order_busywait. Indique en qué condiciones sería apremiante utilizar espera activa, o en caso de no haberlas, sugiera soluciones alternativas.

Agregando un tiempo de espera, los tiempos mejoran significamente. al darle 800000 como parametro el que tienen tiempo de espera termina en 12 segundos mientras que el que no lo tiene no termina y ocupa rapidamente todo el cpu.

modificaciones han sido agregadas: si el programa recibe dos parametros, el primero es el numero de threads y el segundo es el tiempo de espera en microsegundos. Si recibe uno ese es el delay time. Si no recibe nada ejecuta con la cantidad de cores del sistema y un delay time de 50.
[!image](https://git.ucr.ac.cr/ANTHONNY.BADILLA/concurrente22b-antonio_badilaolivas/-/blob/main/Ejercicios/pthreads/delayed_busy_wait_fixed_time/Delayed_Busy_Wait_Number%20of%20Threads_%2010000_.svg)
Discusion: habiendo observado los resultados, parece ser que el busy_wait con 
con tiempo fijo es la solucion mas estable. Asi mismo sus tiempos de ejecucion
fuero en general menores comparados con los de el busy_wait con tiempo random
dado un limite superior. Esto puede deberse a que el tiempo random es por 
definicion impredecible, y por lo tanto no se puede asegurar que los threads 
vayan a esperar el tiempo necesario para que el thread anterior termine, pueden
esperar muy poco y hacer demasiadas demandas al cpu. Mientras que con el tiempo
fijo, la seguridad de tener siempre el limite superior le da estabilidad a los 
tiempos de ejecucion. Ambos casos parecen tener tiempos mas estables conforme
aumentan los tiempos de espera fijos o posibles. 

Sin embargo, definitivamente esta no es una solucion ideal. En ambos casos
la utilizacion del CPU se mantiene en 100% haciendo una simple operacion como 
imprimir un mensaje. Esto es un desperdicio de recursos. Maneras mas eficientes
como espera pasiva o semaforos deberian ser preferidas, dado que cumpliriian
con el objetivo de sincronizar los threads sin hacer uso de recursos 
innecesarios.

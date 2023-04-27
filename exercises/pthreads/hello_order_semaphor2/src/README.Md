Modifique su solución de Ejemplo 7 para inicializar todos los semáforos en 0. Luego haga que el hilo principal incremente el semáforo del hilo 0. ¿Produce el mismo resultado que su solución anterior?
casi el mismo resultado. Lo que pasa es que como todos estan en 0, deben esperar a que el hilo principal haga su signal para empezar los otros. Es como un domino, todos los semaforos estan esperando hasta que el primero se active y ya con uno en verder todos iran
poniendose en verde conforme se ejecuta el programa.

¿Qué pasa si el hilo principal incrementa el semáforo del hilo 0 después de hacer su saludo Hello from main thread?
hace que, con seguridad, el hilo principal siempre imprima primero


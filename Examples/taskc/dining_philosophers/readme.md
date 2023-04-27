

En 1965 Edsger Dijkstra planteó el problema de los filósofos comensales como un ejercicio de un examen para estudiantes. Posteriormente Tony Hoare lo reformuló y se convirtió en un problema clásico de sincronización de procesos en los cursos de sistemas operativos. Existen varias adaptaciones como la que se muestra a continuación considerando algunos aspectos de higiene.

Cinco filósofos, que representan procesos o hilos de ejecución, están sentados a una mesa circular (Figura 6). Los filósofos en silencio piensan y luego comen, y repiten esta rutina eternamente. Ante cada filósofo se encuentra un plato, y al centro de la mesa un tazón con infinito abastecimiento de espagueti. Este abastecimiento representa un recurso compartido, como una cinta magnética en el problema original.
dining philosophers
Figura 6. Filósofos comensales

Para poder comer espagueti, en la mesa están dispuestos cinco tenedores (forks) o palillos chinos en el problema original (chopstiks). Tanto los filósofos como los palillos están enumerados de 0 a 4 como se ve en la figura Figura 6. Para poder comer, un filósofo i necesita los dos palillos (get chopstick), uno a su mano izquierda (i + 1) y el otro a su mano derecha (i), y no los comparte mientras está comiendo. Una vez que haya terminado de comer, el filósofo lava los tenedores y los coloca de regreso en sus posiciones en la mesa (put chopstick). Un filósofo nunca sabe cuándo otro va a comer o pensar.

El reto es construir un algoritmo que permita a los filósofos eternamente pensar y comer que cumpla los siguientes tres requerimientos:

    Los filósofos no se detengan de pensar y comer (bloqueo mutuo o deadlock).

    Ningún filósofo muera de hambre esperando por un palillo (inanición o starvation).

    Dos o más filósofos puedan comer al mismo tiempo (concurrency).

Los filósofos ya saben cómo realizar las operaciones de pensar think() y comer eat(), ninguna de las dos dura eternamente, y se debe detener a un filósofo mientras las realiza.
Ejemplo 23. Filósofos comensales [dining_philosophers]

Corrija el siguiente pseudocódigo de concurrencia de recursos compartidos para que resuelva el problema de los filósofos comensales, y por lo tanto, logre cumplir con los tres requerimientos.

Considere las condiciones que son necesarias para que el bloqueo mutuo o la inanición ocurran, y cambie una de ellas a la vez para generar distintas soluciones:

    Considere la lateralidad de los filósofos.

    Considere la cantidad de filósofos que pueden comer concurrentemente.

    Haga que los filósofos decidan con variables protegidas con exclusión mutua.


Ejecute al menos tres veces los códigos de Ejemplo 7 (imprimir en orden con semáforos) y Ejemplo 8 (imprimir en orden con seguridad condicional) con tantos hilos como su sistema operativo permite. Reporte en su readme.md las tres duraciones de cada solución. Tome la menor duración de las tres corridas de cada versión. ¿Cuál solución realiza su trabajo en menor tiempo? Discuta en máximo un párrafo cuál podría ser la causa de este comportamiento.

En mi caso el condsafe duro menos en general con la misma cantidad de threads.
Pienso que esto es porque semaforos es una solucion que limita la concurrencia para que este en orden
es decir, detiene a los hilos de trabajar en por momentos para que no haya condiciones de carrera.
Mientras que en condsafe cada hilo trabaja en su parcela de memoria sin necesidad de esperar a que otro hilo termine,
solo dejando el trabajo de impresion al hilo final, siendo esta la parte menos costosa de todas.

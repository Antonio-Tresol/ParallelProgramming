El siguiente es un "Hola mundo" en OpenMP. En adelante se usará C++, aunque OpenMP puede usarse perfectamente con C. Considera conceptos como los siguientes:

Instrumentalización de código.

Directivas del preprocesador (pragma): son ignoradas por compiladores en los que no se ha habilitado la instrumentalización.

Región paralela (omp parallel): siempre implica la creación de un equipo de hilos

Barrera o join tras el bloque de instrucciones que conforman la región paralela.

En los Linux OpenMP usa Pthreads internamente. La instrumentalización traslada el código de la región paralela a una rutina, crea hilos con pthread_create(), y espera por ellos con pthread_join().

El hilo principal no puede ejecutar trabajo durante la región paralela, a diferencia de Pthreads.
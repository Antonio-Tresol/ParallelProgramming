- Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE -
# Ejercicio 2 [glossary]

## Conceptos:
**Programación serial**: es la programación en la que se ejecutan las instrucciones una a una, en el orden en que se encuentran escritas en el código fuente.

**Programación concurrente**: es la programación en la que se ejecutan varias instrucciones al mismo tiempo, pero no necesariamente en el mismo orden en que se encuentran escritas en el código fuente.

**Programación paralela**: es la programación en la que se ejecutan varias instrucciones al mismo tiempo, y en el mismo orden en que se encuentran escritas en el código fuente de usando múltiples procesadores o hilos de ejecución.

**Concurrencia de tareas**: "Task parallelism (also known as function parallelism and control parallelism) is a form of parallelization of computer code across multiple processors in parallel computing environments. Task parallelism focuses on distributing tasks—concurrently performed by processes or threads—across different processors. In contrast to data parallelism which involves running the same task on different components of data, task parallelism is distinguished by running many different tasks at the same time on the same data.[1] A common type of task parallelism is pipelining, which consists of moving a single set of data through a series of separate tasks where each task can execute independently of the others."

**Paralelismo de datos**: " Data parallelism is parallelization across multiple processors in parallel computing environments. It focuses on distributing the data across different nodes (or execution threads), which operate on the data in parallel. It can be applied on regular data structures like arrays and matrices by working on each element in parallel."[2]

**Hilo de ejecución**: "In computer science, a thread of execution is the smallest sequence of programmed instructions that can be managed independently by a scheduler, which is typically a part of the operating system."[3] Consiste esencialmente entonces en un vector que contiene el program counter, el stack pointer y los demas regisros que le permitiran ejecutar instrucciones que le corresponden.

**Indeterminismo**: es la propiedad de un sistema que no puede predecir el resultado de una operación o de un proceso, debido a que el resultado depende de factores externos al sistema.

**Memoria privada y compartida**: en un sistema operativo, la memoria privada es la memoria que cada proceso o hilo tiene asignada para su uso exclusivo. La memoria compartida es la memoria que se comparte entre varios procesos o hilos.

**Espera activa**: significa que el programa esta esperando, pero para realizar esa espera esta ejecutando constantemente una instruccion que verifica si la condicion de espera se cumple es decir, esta realizando un bucle infinito que gastara recursos del sistema.

**Condición de carrera**: cuando dos o mas hilos de ejecución acceden a una misma variable compartida y al menos uno de ellos escribe en ella, se dice que existe una condición de carrera. Si los hilos de ejecución no se sincronizan adecuadamente, el resultado de la ejecución puede depender del orden en que se ejecutan los hilos de ejecución.
la condicion de carrera es entonces un modificacion concurrente a memoria compartida.

**Control de concurrencia**: son diferentes mecanismo que se utilizan para evitar condiciones de carrera, como por ejemplo el uso de semaforos, candados (mutex), wrlocks, barreras, etc.

**Seguridad condicional**: se refiere a cuando un programa, por la forma en que ha distribuido los datos entre hilos, esta asegurando que los datos no sean accesibles por otros hilos. Esto hace que logicamente no exista una condicion de carrera y no se necesite  control de concurrencia. 

**Exclusión mutua**: es un mecanismo que se utiliza para evitar condiciones de carrera. Consiste en que un hilo de ejecución que quiere acceder a una variable compartida, primero debe asegurarse que no haya otro hilo de ejecución que este accediendo a la misma variable. Para esto se utiliza un candado (mutex) que se utiliza para asegurar que solo un hilo de ejecución pueda acceder a la variable compartida.

**Semáforo**: es un mecanismo de control de concurrencia que permite a un hilo de ejecución esperar a que otro hilo de ejecución realice una operación. Por ejemplo, un hilo de ejecución puede esperar a que otro hilo de ejecución termine de escribir en una variable compartida. Cuando el hilo 1 termina de escribir en la variable compartida, hace signal al semáforo, y el hilo 2 que estaba esperando al semáforo, puede continuar su ejecución. esto tambien sirve para coordinar la ejecucion de varios hilos de ejecucion.

**Barrera**: es un mecanismo de control de concurrencia el cual solo deja pasar a los hilos o procesos cuando todos han llegado a esa parte.

**Variable de condición**: es un mecanismo de control de concurrencia parecido a una barrera que deja pasar a todos los procesos o hilos de ejecuccion cuando se le indica. La diferencia es que en este caso, los hilos de ejecucion no se quedan esperando a que todos lleguen, sino que se quedan esperando a que se les indique que pueden continuar. luego de eso tienen un momento para pasa y la "barrera" se cierra de nuevo hasta que se vuelva a 

**Candado de lectura y escritura**:  "In computer science, a readers–writer ( also known in the cientific literature as single-writer lock, a multi-reader lock, a push lock, or an MRSW lock ) is a synchronization primitive that solves one of the readers–writers problems. An RW lock allows concurrent access for read-only operations, write operations require exclusive access. This means that multiple threads can read the data in parallel but an exclusive lock is needed for writing or modifying data. When a writer is writing the data, all other writers or readers will be blocked until the writer is finished writing. A common use might be to control access to a data structure in memory that cannot be updated atomically and is invalid (and should not be read by another thread) until the update is complete."[4]

**Descomposición**: es la division de un problema en subproblemas mas pequeños que pueden ser resueltos de forma independiente.

**Mapeo**: son tecnicas que permiten distribuir unidades de trabajo entre procesos o hilos de ejecucion. (por ejemplo mapeo estatico por bloque, mapeo dinamico, mapeo ciclico, etc.)

**Incremento de velocidad**: se refiere a cuanto un programa gana velocidad con respecto a la version serial, se calcula como (duracion de la version serial)/(duracion de la version paralela).

**Comunicación punto a punto entre procesos**: en computacion distribuida, la comunicacion punto a punto es un tipo de comunicacion entre procesos que se realiza entre dos procesos especificos. [5]

**Comunicación colectiva entre procesos**: en computacion distribuida, la comunicacion colectiva es un tipo de comunicacion entre procesos que se realiza entre todos los procesos de un grupo. [6]

**Reducción**: "In functional programming, fold (also termed reduce, accumulate, aggregate, compress, or inject) refers to a family of higher-order functions that analyze a recursive data structure and through use of a given combining operation, recombine the results of recursively processing its constituent parts, building up a return value. Typically, a fold is presented with a combining function, a top node of a data structure, and possibly some default values to be used under certain conditions. The fold then proceeds to combine elements of the data structure's hierarchy, using the function in a systematic way."[7]


## **Referencias:**
1. Quinn, Michael J. (2007). Parallel programming in C with MPI and openMP (Tata McGraw-Hill ed.). New Delhi: Tata McGraw-Hill Pub. ISBN 978-0070582019.
2. Pacheco, P., & Malensek, M. (2020). An Introduction to Parallel Programming (2nd ed.). Morgan Kaufmann.
3. Lamport, Leslie (September 1979). "How to Make a Multiprocessor Computer That Correctly Executes Multiprocess Programs" (PDF). IEEE Transactions on Computers. C-28 (9): 690–691. doi:10.1109/tc.1979.1675439. S2CID 5679366. 
4. Raynal, M. (2012). Concurrent Programming: Algorithms, Principles, and Foundations (2013th ed.). Springer.
5.6. Kshemkalyani, A. D., & Singhal, M. (2011). Distributed Computing: Principles, Algorithms, and Systems (Illustrated). Cambridge University Press.
1. Bird, R. (2010). Pearls of Functional Algorithm Design. Cambridge University Press.

## **lectura recomendada**: 
Programming Massively Parallel Processors
A Hands-on Approach
By David B. Kirk and Wen-mei W. Hwu
ISBN: 9780123814722

The Art of Multiprocessor Programming
By Maurice Herlihy and Nir Shavit
ISBN: 9780123705914

Parallel Programming with MPI
By Peter Pacheco
ISBN: 9781558603394

The Sourcebook of Parallel Computing
Edited by Jack Dongarra et al.
ISBN: 9781558608719

Parallel Computer Architecture
A Hardware/Software Approach
By David Culler, J. P. Singh and Anoop Gupta
ISBN: 9781558603431

Engineering a Compiler, Second Edition
By Keith D. Cooper and Linda Torczon
ISBN: 9780120884780

M. van Steen and A.S. Tanenbaum, Distributed Systems, 3rd ed., distributed-systems.net, 2017.

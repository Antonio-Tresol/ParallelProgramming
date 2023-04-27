# REPORT
# Student:
- Author: A Badilla Olivas
- Student ID: B80874
## Test Machine:
- ARENAL cluster
## Test methodology:
- The program was tested using the test cases provided by the professor.
- Time was measured using the command time in the terminal.
- Each test was repeated 10 times, best 3 times were stored on [Goldbach Execution Times](https://docs.google.com/spreadsheets/d/10d7yxW-iVHz6fpZRmVMwo5TuA5NqgiKkBZ6PQkxeKaA/edit?usp=sharing)
- System was on minimal load, no other programs were running.
## Optimization 1:
- In this optimization the goal was to distribute the work between threads dynamically.(see more in report in goldbach_optimization)
 
## Optimization 2.1:
 
- adding a break when the result is negative. (see more in report in goldbach_optimization)
Before this optimization 2.1 and optimization 1, goldbach_pthread was taking ~ 0.3774 seconds to compute all tests (medium and small) on average. After the optimization execution time was reduced to ~ 0.33987 seconds giving a speed up of 1.111.
 
## Optimization 2.2:
- After observation and revision, it was noted that when the result was less than prime (for even numbers) or less than second (for odd numbers) there was no need to continue iterating. (see more in report in goldbach_optimization)
 
Before this optimization, goldbach_pthread was taking ~ 0.3774 seconds to compute all tests (medium and small) on average. After the optimization execution time was reduced to ~ 0.157626 seconds giving a speed up of 2.394. Most of the time, this optimization reduces the execution time in half for any number of numbers tested.
 
For more information about the execution time of the program, see [Goldbach Execution Times](https://docs.google.com/spreadsheets/d/1jb-0_jDBswWkYpWAfo9KhPag5tq6eycKZbnXduzNVnI/edit?usp=sharing) (this are from goldbach_optimization in test machine - Processor: Intel(R) Core(TM) i5-12600k CPU @ 3.60GHz (6 p-cores, 6 e-cores) and RAM: 32gb ddr4 3200mhz)
 
## Optimization Comparison from goldbach_optimization:
(see more in report in goldbach_optimization)
## Comparison: (TEST MACHINE ARENAL)
As shown in the optimization graph in terms of duration, golbach_optimization is the fastest of all of the versions tested. This version, must be pointed out, is not far ahead of the openmp implementation with dynamic scheduling. Another important element to point out is that the MPI version, when run in a single machine with 1 process only has an equivalent duration to the serial version. This may be due to the time taken to initialize the MPI environment and the time taken to communicate between processes.
 
![image](/design/OptimizationGraph.png)
 
With respect to the concurrency graph it can be noted that the results resembled what is already exposed in the [goldbach_optimization/report].
In this regard, there is an increase in speed from the serial version when more threads are used, nevertheless as the ratio thread/core increases there are diminishing returns in speed up. It is interesting that, the MPI version has a significant speed up when executed in a cluster of machines. This is due to the fact that the MPI version is able to use all the cores of the machines in the cluster. This is not the case of the pthread version, which is limited to the number of cores of the machine where it is executed.
 
![image](/design/ConcurrencyGraph.png)
 
Nevertheless, it is important to note that as more threads are used, less efficiency there is. The equilibrium between speed up and efficiency is reached when the number of threads is equal to half the number of cores of the machine at least based on this data.
## Conclusion:
 
Therefore the optimization is the best optimization for the goldbach program. It is the fastest and the most efficient. Nevertheless, the openmp version is very close in speed for far less effort when developing the solution. It must be said that as depicted by the graphs there are diminishing returns when more threads are used. One must carefully analyze the needs to solve a problem but also consider the cost of the solution. Even when the best performance was achieved in this example with 8 threads, it might be worth considering, depending on the context, using 4 or 3 as there lies the balance between speed up and efficiency. Also, it can be noted that the MPI version excels when executed in a cluster of machines. This is due to the fact that the MPI version is able to use all the cores of the machines in the cluster.

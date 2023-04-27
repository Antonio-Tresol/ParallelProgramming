// Copyright CC-BY 4.0
#include <pthread.h>  //library standar posix to handle threads on 
#include <stdio.h>
#include <stdlib.h>
//procedure greet
//print "hello world from secondary thread"
void* greet(void* data); //procedure greet
  //procedure main
int main(void) { //main procedure
    //create_thread(greet) #Comment: delego tareas, pido 
    //print "hello from the main thread"
    pthread_t thread;
    int error = pthread_create(&thread, /*attributs changes priority threads*/ NULL,/*subroutine*/ greet, /* *arg */ NULL);  //pthread_create returns an error code that's why we need and int to put it 
    if (error == EXIT_SUCCESS) {    
        printf("hello from main thread");
        pthread_join(thread,/*value ptr puntero a donde se guarda un puntero de retorno de la subrutina ejecutada*/ NULL);  //we need to close the thread and free and return the memory.
        //at first we had an error here en we used 8 different tools like warnings, lintes, valgrind and sanitizers to identify the error.
        //join pone en cola de espera al hilo principal para que luego se junten y luego se cierran juntos.
    } else {  //rainy day
        fprintf(stderr,"error creating thread");  //ceerr
    }
    return error;

}  //end procedure

void* greet(void* data) { //
    printf("\nhello world from secundary thread");
    return NULL;
}



#include <stdio.h>
#include <stdlib.h>

//placeholder for crearting new processes - we may want other logic
int newProcess() {

    int newProcess = fork();

    return newProcess;
}

//placeholder for cleaning memory associated with this code
void cleanProcesses(int* processes) {

    int length = sizeof(processes) / sizeof(int);

    free(processes);
}

//returns pointer to array of child pids
int* createChildProcesses( int numProcesses) {
    
    int* processIds = (int*) malloc(sizeof(int) * numProcesses);

    for( int i = 0 ; i < numProcesses ; i++) {
        
        int processId = newProcess();

        if(processId < 0){
            fprintf(stderr , "unable to execute command exiting...");
            exit(1); //it failed - ooooooooooof
        }
        
        processIds[i] = processId;
    }

}

//returns pointer to array of 2 element arrays - MUST FREE ALL ARRAY SLOTS AND THEN FREE TOPLEVEL ARRAY ON DESTROY
//the array is all of the 
int* createIoBuffers(int numProcesses) {
    
    int* processes = (int*) malloc((numProcesses - 1) * sizeof(int));

    for( int i = 0 ; i < numProcesses - 1 ; i++){
        processes[i] = (int*) malloc(2 * sizeof(int));
    }

    return processes;
}

//assumes we are inside a child process - DO NOT CALL IN PARENT , or do idk im not ur mom
void reassignFileHandlers( int* processIds ) {

    //because i dont trust you
    int length =  sizeof(processIds) / sizeof(int);

    for( int i = 0 ; i < length ; i++) {
        
        if(processIds[i] != 0) { //inside parent
            return;
        }
    }

    //assumed to be inside child process

    int *

}

pipe()
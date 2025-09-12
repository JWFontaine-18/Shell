#include <stdio.h>
#include <stdlib.h>

//placeholder for crearting new processes - we may want other logic
int newProcess() {

    int newProcess = fork();

    return newProcess;
}

//placeholder for cleaning memory associated with this code
//pipes is assumed to take the form int[ int[2] , int[2] ]
void cleanProcesses(int* processes , int** pipes) {

    int processesLength = sizeof(processes) / sizeof(int);
    int pipesLength = sizeof(pipes) / sizeof(int);

    free(processes);

    for(int i = 0 ; i < pipesLength ; i++) {
        free(pipes[i]);
    }

    free(pipes);
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

    return processIds;

}

//returns pointer to array of 2 element arrays - MUST FREE ALL ARRAY SLOTS AND THEN FREE TOPLEVEL ARRAY ON DESTROY
//the array is all of the 
int** createIoBuffers(int numProcesses) {
    
    int** processes = (int**) malloc((numProcesses - 1) * sizeof(int*));

    for( int i = 0 ; i < numProcesses - 1 ; i++){
        processes[i] = (int*) malloc(2 * sizeof(int));
    }

    return processes;
}

//assumes we are inside a child process - DO NOT CALL IN PARENT , or do idk im not ur mom
//returns an array of pipes - each pipe is an array of 2 ints
int** createPipes( int* processIds ) {

    //because i dont trust you
    int length =  sizeof(processIds) / sizeof(int);

    for( int i = 0 ; i < length ; i++) {
        
        if(processIds[i] != 0) { //inside parent
            return;
        }
    }

    //assumed to be inside child process

    int** buffers = createIoBuffers(length);

    for(int i = 0 ; i < length - 1; i++) {

        int success = pipe(buffers[i]);

        if(success == -1) {
            fprintf(stderr , "unable to process command. Exiting...");
            exit(1);
        }
    }

    return buffers;
}

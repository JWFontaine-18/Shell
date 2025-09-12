#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

//placeholder for crearting new processes - we may want other logic
int newProcess() {

    int newProcess = fork();

    return newProcess;
}

//placeholder for cleaning memory associated with this code
//pipes is assumed to take the form int[ int[2] , int[2] ]
void cleanProcesses(int* processes , int processCount , int** pipes , int pipesCount) {

    free(processes);

    for(int i = 0 ; i < pipesCount ; i++) {
        free(pipes[i]);
    }

    free(pipes);
}

//returns pointer to array of child pids
int* createChildProcesses( int numProcesses) {
    
    int* processIds = (int*) malloc(sizeof(int) * numProcesses);

    for( int i = 0 ; i < numProcesses ; i++) {
        
        int processId = newProcess();

        if(processId == 0 ) {
            return NUll;
        }

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

        int success = pipe(processes[i]);

        if(success == -1) {
            fprintf(stderr , "unable to process command. Exiting...");
            exit(1);
        }
    }

    return processes;
}

//assumes we are inside a child process - DO NOT CALL IN PARENT , or do idk im not ur mom
//returns an array of pipes - each pipe is an array of 2 ints
int** createPipes( int* processIds , int idsLength) {

    for( int i = 0 ; i < idsLength ; i++) {
        
        if(processIds[i] != 0) { //inside parent
            return NULL;
        }
    }

    //assumed to be inside child process

    //returns an array of buffers of the length needed to link the processes together
    int** buffers = createIoBuffers(idsLength);

    return buffers;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

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
//the array commands contains the commands to be executed with piping in order , num commnads in the number of them
int createChildProcesses(char* commands , int numCommands) {
    
    int* processIds = (int*) malloc(sizeof(int) * (numCommands));

    int** processPipes = createPipes(numCommands); //has number of pipes needed for communication
    int numPipes = sizeof(processPipes) / sizeof(int);

    for( int i = 0 ; i < (numCommands) ; i++) {
        
        int processId = fork();

        if(processId < 0){
            fprintf(stderr , "unable to execute command exiting...");
            exit(1); //it failed - ooooooooooof
        }

        if(processId == 0 ) { //inside child process
            
            //execute commands as needed
        }
        else { //inside parent
            //await completion
        }
        
        processIds[i] = processId;
    }

    //free allocated memory
    cleanProcesses(processIds , numCommands , processPipes , numPipes);

    return 1;

}

//do not call inside child
//must free return value of function when done
int** createPipes( int numProcesses) { //for n procecesses we need n-1 pipes

    //assumed to be inside parent process
    int** processes = (int**) malloc((numProcesses - 1) * sizeof(int*));

    for( int i = 0 ; i < numProcesses - 1 ; i++){ //

        processes[i] = (int*) malloc(2 * sizeof(int));

        int success = pipe(processes[i]);

        if(success == -1) {
            fprintf(stderr , "unable to process command. Exiting...");
            exit(1);
        }
    }

    //returns an array of buffers of the length needed to link the processes together

    return processes;
}

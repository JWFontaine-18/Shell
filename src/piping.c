#include <stdio.h>
#include <unistd.h>
#include "piping.h"
#include <sys/types.h>
#include <sys/wait.h>
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
//WILL NEED TO MODIFY TO SUPPORT BACHGROUND PROCESSES
//args is a 2-d array of arguments to the respective commands. empty array if none. same length as commands
int createChildProcesses(char** commands , const int numCommands , char*** args) {
    
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

            if( i == 0 ) {

                dup2(processPipes[0][1] , STDOUT_FILENO); //replace stdout with write end of pipe

                //first subproces now has write end of pipe as stdout
            }
            else if( i == numCommands - 1) { //last command goes to stdout EDIT TO SUPPORT REDIRECTS MAYBE

                dup2(processPipes[ i - 1][0] , STDIN_FILENO); //takes input from previous command

                //final subprocess now takes input from previous process and puts it to stdin
            }
            else { //any inbetween
                dup2(processPipes[ i - 1][0] , STDIN_FILENO); //takes input from previous
                dup2(processPipes[i][1] , STDOUT_FILENO); //puts output to next
            }

            execv(commands[i] , args[i] );

        }
        else { //inside parent
            
            //await completion
        }
        
        processIds[i] = processId;
    }

    for(int i = 0 ; i < numCommands ; i++) {
        waitpid(processIds[i] , NULL ,  0); //TODO: WILL NEED TO EXPAND FOR BACKGROUND PROC
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

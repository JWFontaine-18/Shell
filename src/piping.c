#include <stdio.h>
#include <unistd.h>
#include "piping.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


//returns pointer to array of child pids
//the array commands contains the commands to be executed with piping in order , num commnads in the number of them
//WILL NEED TO MODIFY TO SUPPORT BACHGROUND PROCESSES
//args is a 2-d array of arguments to the respective commands. empty array if none. same length as commands
void createChildProcesses(char** commands , const int numCommands , char*** args) {

    int pipes[2][2];

    int pids[numCommands]; 

	// create read and write end of pipe
	pipe(pipes[0]);
	pipe(pipes[1]);


    for( int i = 0 ; i < numCommands ; i++) {

        //printf("%s\n" , args[i][0]);

        int pid = fork();

        pids[i] = pid;

        if(pid < 0) {
            printf("Error: unable to execute command\n");
            exit(1);
        }

        if(pid == 0 ) {

            if(i > 0 ) {
                dup2(pipes[i-1][0] , STDIN_FILENO);
            }
            
            if( i < numCommands - 1) {

                dup2(pipes[i][1] , STDOUT_FILENO);
            }


            for(int j = 0 ; j < 2 ; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            int success = execv(commands[i] , args[i]);

            if(success == -1) {
                printf("Error: Unable to execute");
                exit(1);
            }
        }

    }

    for(int j = 0 ; j < 2 ; j++) { //oh my god the parent has to close them too
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

    for(int i = 0 ; i < numCommands ; i++) {
        waitpid(pids[i] , NULL , 0);
    }

    return;

}
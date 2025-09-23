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

    char* arg[2];

	// create read and write end of pipe
	pipe(pipes[0]);
	pipe(pipes[1]);


    for( int i = 0 ; i < numCommands ; i++) {

        arg[0] = commands[i];
        arg[1] = NULL;

        printf("%s\n" , commands[i]);

        int pid = fork();

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


            close(pipes[i][0]);
            close(pipes[i][1]);


            int success = execv(commands[i] , arg);

            if(success == -1) {
                printf("Error: Unable to execute");
                exit(1);
            }
        }

    }

    return;

}
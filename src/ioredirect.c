#include "command_utils.h"
#include "stdlib.h"
#include "stdio.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//allows redirect commands to be in defined format
struct redirectCommand {
    char* command;
    char* infilePath;
    char* outfilePath;
};

void freeRedirectCommand(struct redirectCommand* comm) {
    free(comm->command);
    free(comm->infilePath);
    free(comm->outfilePath);
}

//free allocated memory
void cleanRedirect() {

}

//you can just pass the direct command sequence here - we will parse it
//we do assume it has at least ONE I/O redirect in it tho
//we will validate command structure
int redirectInput(const char** input , int inputLength) {

    int numCommands = 1; //TODO make accurate
    struct redirectCommand** redirects = parseInput(input , inputLength , numCommands);

    for( int i = 0 ; i < numCommands ; i++) { //TODO: generalize and make work

        struct redirectCommand* command = redirects[i];

        int procId = fork();

        if(procId < 0) {
            printf('fatal error executing command');
            exit(1);
        }
        else if(procId == 0 ) { //inside child

            int infileDesc = -1;
            int outfileDesc = -1;

            if(command->infilePath) {

                infileDesc = fopen(command->infilePath , "r"); //TODO: error if file does not exist

                //TODO: error check

                dup2(infileDesc , STDIN_FILENO);
                
            }

            if(command->outfilePath) {
                
                outfileDesc = fopen(command->outfilePath , 'w');

                //TODO: error check

                dup2(outfileDesc , STDOUT_FILENO);
            }

            execv(command->command , NULL);
            
        }
        else {
            waitpid(procId , NULL , 0); //TODO: refactor for background execution
        }
        
    }
    
}

//expects commands to be in order of input
struct redirectCommand** parseInput(char** input , int inputLength , int numCommands) {

    //we use an array of structs as a placeholder in case we need to support multiple commands
    struct redirectCommand** redirectCommands = (struct redirectCommand*) malloc(sizeof( struct redirectCommand* ) * numCommands);
    char* firstCommandPath;

    for( int i = 0 ; i < numCommands ; i++) { //initialize commands with empty structs
        redirectCommands[i] = (struct redirectCommand*) malloc(sizeof(struct redirectCommand));
        redirectCommands[i]->command = NULL;
        redirectCommands[i]->infilePath = NULL;
        redirectCommands[i]->outfilePath = NULL;
    }

    if(!command_exists(input[0])) {
        printf("fatal parse error"); //we should never get here, it is expected that input contains a starting command when passed
        exit(1);
    }

    firstCommandPath = get_command_path(input[0]); //first argument should be a command

    //TODO: generalize
    redirectCommands[0]->command = firstCommandPath;

    for(int i = 0 ; i < inputLength ; i++) { //assumes only one command in the pipe for now TODO: expand
        
        if( i == 0){
            continue;
        }

        if(input[i] == '>' || input[i] == '<') {

            if(i >= inputLength) { //error state
                return NULL;
            }

            if(input[i] == '>') { //TODO: logic here breaks if i add a > || < at the end of input
                
                i++;

                redirectCommands[0]->outfilePath = input[i]; //TODO: make support multiple commands

                i++;
            }
            else if(input[i] == '<') {

                i++;

                redirectCommands[0]->infilePath = input[i]; //TODO: make support multiple commands

                i++;
            }
        }
    }
}
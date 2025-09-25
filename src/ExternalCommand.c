#include "ExternalCommand.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <unistd.h> // Required for fork()
#include <sys/types.h> // Required for pid_t (process ID type)
#include <sys/wait.h> // Required for wait() or waitpid()
#include "background.h"

void ExternalCommand(tokenlist * tokens , backgroundProcs* backgroundProcesses , int sendtoBack) {

    if (tokens->size > 0 && tokens->items[0]) {                     
            const char *cmd = tokens->items[0];                         

            if (!is_builtin(cmd)) {                                    
                char *prog_path = search_path_for_command(cmd);         

                if (!prog_path) {                                       
                    if (strchr(cmd, '/') == NULL)                        
                        fprintf(stderr, "command not found: %s\n", cmd); 
                    // If user provided a direct path that isn't executable,
                    // search_path_for_command returns NULL as well.       
                } else {

                    char** args = (char**) malloc(sizeof(char*) * (tokens->size + 1)); //+1 for NULL on args

                    for(int i = 0 ; i < tokens->size ; i++) {

                        if(i == 0 ) {
                            args[i] = prog_path;
                            continue;
                        }

                        args[i] = tokens->items[i]; //pointer copy is fine here, only thing we will need to free is top pointer and prog path
                    }

                    args[tokens->size] = NULL; //null term for execv

                    pid_t pid = fork(); 

                    if (pid < 0) {

                        perror("fork");   
                                                       
                    } else if (pid == 0) {

                        // Child: replace process image with the program  
                        execv(prog_path, args); 

                        // If execv returns, it failed                    
                        perror("execv");  

                        _exit(127);   

                    } else {

                        // Parent: wait for the child to finish           
                        int status = 0;   

                        if(sendtoBack) {

                            addBackgroundProcess(backgroundProcesses , pid , tokens);

                            if(waitpid(pid , &status , WNOHANG) == -1) {
                                //TODO: error case
                            }
                        }
                        else {
                            if (waitpid(pid, &status, 0) < 0)                
                                perror("waitpid");  
                        }                         
                    }


                    free(args);
                    free(prog_path);                                    
                }
            }
        }
}
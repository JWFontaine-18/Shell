#include "ExternalCommand.h"
#include "path_search.h"

void ExternalCommand(tokenlist * tokens) {
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
                    pid_t pid = fork();                                  
                    if (pid < 0) {
                        perror("fork");                                  
                    } else if (pid == 0) {
                        // Child: replace process image with the program  
                        execv(prog_path, tokens->items);                 
                        // If execv returns, it failed                    
                        perror("execv");                                 
                        _exit(127);                                   
                    } else {
                        // Parent: wait for the child to finish           
                        int status = 0;                                 
                        if (waitpid(pid, &status, 0) < 0)                
                            perror("waitpid");                           
                    }
                    free(prog_path);                                    
                }
            }
        }
}
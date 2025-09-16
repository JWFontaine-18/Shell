
#include "command_utils.h"
#include "path_search.h"
#include <stdlib.h>


// Return 1 if a command exists and is executable.

int command_exists(const char *cmd) {
    if (!cmd || *cmd == '\0') return 0;

    // Builtins count as "existing"
    if (is_builtin(cmd)) return 1;

    char *path = search_path_for_command(cmd);
    if (path) {
        free(path);
        return 1;
    }
    return 0;
}

 // Return path to a command or NULL.
 
char *get_command_path(const char *cmd) {
    if (!cmd || *cmd == '\0') return NULL;

    if (is_builtin(cmd)) return NULL;

    return search_path_for_command(cmd);  // may be NULL
}

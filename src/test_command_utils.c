#include "../include/command_utils.h"
#include <stdio.h>
#include <stdlib.h>

int run_command_utils_tests(void) {
    const char *tests[] = {"ls", "echo", "nosuchcmd", "cd", NULL};

    for (int i = 0; tests[i]; i++) {
        const char *cmd = tests[i];
        printf("Testing: %s\n", cmd);

        printf("  command_exists(\"%s\") = %d\n", cmd, command_exists(cmd));

        char *p = get_command_path(cmd);
        if (p) {
            printf("  get_command_path(\"%s\") = %s\n", cmd, p);
            free(p);
        } else {
            printf("  get_command_path(\"%s\") = (null)\n", cmd);
        }
    }

    return 0;
}

#include "minishell.h"

#define MAX_PATH_LENGTH 1024

char *get_path() {
    return getenv("PATH");
}

void try_executing(char *command, char *path, char *token_content) {
    char full_path[MAX_PATH_LENGTH];

    // Copy the original path so we don't modify it
    char path_copy[MAX_PATH_LENGTH];
    strncpy(path_copy, path, sizeof(path_copy));

    while (token_content != NULL) {
        // Build the full path by concatenating the directory and the command
        strcpy(full_path, token_content);
        strcat(full_path, "/");
        strcat(full_path, command);

        // Try executing the command with the full path
        if (access(full_path, X_OK) == 0) {
            char *args[] = {command, NULL};
            char *env[] = {NULL};  // Environment variables (empty for this example)

            execve(full_path, args, env);

            // If execve succeeds, the following code won't be reached
            perror("execve"); // Print an error message if execve fails
            exit(EXIT_FAILURE);
        }
    }

    // If we reach this point, the command was not found in any of the directories
    fprintf(stderr, "Command not found: %s\n", command);
    exit(EXIT_FAILURE);
}

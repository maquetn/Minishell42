#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 1024

char *get_path() {
    return getenv("PATH");
}

void try_executing(char *command, char *path) {
    char full_path[MAX_PATH_LENGTH];
    char *token;

    // Copy the original path so we don't modify it
    char path_copy[MAX_PATH_LENGTH];
    strncpy(path_copy, path, sizeof(path_copy));

    // Tokenize the PATH variable
    token = strtok(path_copy, ":");

    while (token != NULL) {
        // Build the full path by concatenating the directory and the command
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);

        // Try executing the command with the full path
        if (access(full_path, X_OK) == 0) {
            char *args[] = {command, NULL};
            execv(full_path, args);

            // If execv succeeds, the following code won't be reached
            perror("execv"); // Print an error message if execv fails
            exit(EXIT_FAILURE);
        }

        // Move to the next token in the PATH
        token = strtok(NULL, ":");
    }

    // If we reach this point, the command was not found in any of the directories
    fprintf(stderr, "Command not found: %s\n", command);
    exit(EXIT_FAILURE);
}

int main() {
    char *command = "ls";
    char *path = get_path();

    if (path == NULL) {
        fprintf(stderr, "PATH variable not set\n");
        return EXIT_FAILURE;
    }

    try_executing(command, path);

    return 0; // This line is not reached if execv is successful
}

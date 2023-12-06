#include "minishell.h"

#define MAX_PATH_LENGTH 1024

char *get_path() {
    return getenv("PATH");
}

int try_executing(char *command, char *path) {
    char full_path[MAX_PATH_LENGTH];
    char *path_ptr = path;

    pid_t pid = fork();  // Fork a new process

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        while (*path_ptr != '\0') {
            // Find the next colon (:) in the path
            char *end = strchr(path_ptr, ':');

            // If no colon is found, set end to the end of the path string
            if (end == NULL) {
                end = path_ptr + strlen(path_ptr);
            }

            // Copy the directory from path_ptr to end into full_path
            strncpy(full_path, path_ptr, end - path_ptr);
            full_path[end - path_ptr] = '\0';

            // Build the full path by concatenating the directory and the command
            strcat(full_path, "/");
            strcat(full_path, command);

            // Try executing the command with the full path
            if (access(full_path, X_OK) == 0) {
                char *args[] = {command, NULL};
                char *env[] = {NULL};  // Environment variables (empty for this example)

                execve(full_path, args, env);

                // If execve succeeds, the following code won't be reached
                perror("execve"); // Print an error message if execve fails
                exit(EXIT_FAILURE);  // Exit the child process on execve failure
            }

            // Move to the next directory in the PATH
            path_ptr = (*end == ':') ? end + 1 : end;
        }

        // If we reach this point, the command was not found in any of the directories
        fprintf(stderr, "Command not found: %s\n", command);
        exit(EXIT_FAILURE);  // Exit the child process if the command is not found
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);  // Wait for the child process to complete

        // Check if the child process exited successfully
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            return 1;  // Return 1 to indicate the command was found and executed
        } else {
            return 0;  // Return 0 to indicate an error or that the command was not found
        }
    }
}

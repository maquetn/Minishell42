#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int execute_command(char *command) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        char *args[] = {command, NULL};

        // Specify the full path to the command or leave it as is
        char *full_path = "/bin/";  // You can modify this based on your needs
        char full_command[strlen(full_path) + strlen(command) + 1];
        strcpy(full_command, full_path);
        strcat(full_command, command);

        execve(full_command, args, NULL);

        // If execve fails, print an error message and exit with status 127
        perror("execve");
        exit(127);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            // Return the exit status of the child process
            return WEXITSTATUS(status);
        } else {
            // The child process did not exit normally
            return -1;
        }
    }
}

void execute_command_and_print_result() {
    char command[100];

    printf("Enter a command to execute: ");
    scanf("%s", command);

    // Check if the command is "exit"
    if (strcmp(command, "exit") == 0) {
        printf("Exiting the program.\n");
        exit(0);
    }

    int exit_status = execute_command(command);

    if (exit_status != -1) {
        printf("Exit status of the last command: %d\n", exit_status);
    } else {
        printf("Error executing command: %s\n", command);
        exit_status = 127;
    }
}

int main() {
    execute_command_and_print_result();

    return 0;
}

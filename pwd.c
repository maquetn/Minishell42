#include "minishell.h"
char* get_current_directory()
{
    char* cwd = malloc(PATH_MAX);
    if (cwd == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    if (getcwd(cwd, PATH_MAX) == NULL)
    {
        perror("getcwd");
        free(cwd);
        exit(EXIT_FAILURE);
    }

    return cwd;
}
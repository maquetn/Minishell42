#include "minishell.h"

char* ft_pwd()
{
    char* cwd = malloc(PATH_MAX);
    if (cwd == NULL)
    {
        perror("malloc");
        ft_exit(EXIT_FAILURE);
    }

    if (getcwd(cwd, PATH_MAX) == NULL)
    {
        perror("getcwd");
        free(cwd);
        ft_exit(EXIT_FAILURE);
    }

    // Check if the current directory is the root directory
    if (strcmp(cwd, "/") == 0)
    {
        free(cwd);
        return strdup("/");
    }

    return cwd;
}

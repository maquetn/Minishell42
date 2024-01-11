#include "../minishell.h"

char* ft_pwd()
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
    if (strcmp(cwd, "/") == 0)
    {
        free(cwd);
        return strdup("/");
    }

    return cwd;
}

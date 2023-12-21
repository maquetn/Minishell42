#include "minishell.h"

int ft_cd(char *token)
{
    char cwd[PATH_MAX];

    printf("Token: %s\n", token);

    if (token == NULL || token[0] == '\0')
    {
        char *home_dir = getenv("HOME");

        if (home_dir == NULL)
        {
            fprintf(stderr, "Home directory not found.\n");
            return 0;
        }

        if (chdir(home_dir) == 0)
        {
            printf("Changed to home directory: %s\n", home_dir);
            return 1;
        }
        else
        {
            perror("chdir");
            fprintf(stderr, "Error changing to home directory: %s\n", home_dir);
            return 0;
        }
    }
    else if (strcmp(token, "..") == 0)
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd");
            return 0;
        }

        if (strcmp(cwd, "/") == 0)
        {
            printf("Already at the root directory.\n");
            return 1;
        }

        if (chdir("..") == 0)
        {
            printf("Changed to parent directory.\n");
            return 1;
        }
        else
        {
            perror("chdir");
            fprintf(stderr, "Error changing to parent directory.\n");
            return 0;
        }
    }
    else
    {
        if (chdir(token) == 0)
        {
            if (getcwd(cwd, sizeof(cwd)) == NULL)
            {
                perror("getcwd");
                return 0;
            }

            printf("Changed to: %s\n", cwd);
            return 1;
        }
        else
        {
            perror("chdir");
            fprintf(stderr, "Error changing to directory: %s\n", token);
            return 0;
        }
    }
}

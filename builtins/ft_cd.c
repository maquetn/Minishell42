#include "minishell.h"

int ft_cd(t_token *tokens)
{
    char cwd[PATH_MAX];

    if (tokens == NULL || tokens->content == NULL)
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
            perror("cd");
            return 0;
        }
    }
    else if (strcmp(tokens->content, "..") == 0)
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
            return 1;
        else
        {
            perror("cd");
            return 0;
        }
    }
    else
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd");
            return 0;
        }

        if (chdir(tokens->content) == 0)
        {
            char *target_path = getcwd(NULL, 0);

            if (target_path == NULL)
            {
                perror("getcwd");
                chdir(cwd);
                return 0;
            }

            printf("Changed to: %s\n", target_path);
            chdir(cwd);
            free(target_path);

            return 1;
        }
        else
        {
            perror("cd");
            return 0;
        }
    }
}

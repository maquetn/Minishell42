#include "minishell.h"

int ft_cd(t_token *tokens)
{
    if (tokens != NULL && tokens->content != NULL)
    {
        char *target_path;

        if (strcmp(tokens->content, "..") == 0)
        {
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
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) == NULL)
            {
                perror("getcwd");
                return 0;
            }

            if (chdir(tokens->content) == 0)
            {
                target_path = getcwd(NULL, 0);

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
    else
    {
        fprintf(stderr, "Invalid token for cd\n");
        return 0;
    }
}

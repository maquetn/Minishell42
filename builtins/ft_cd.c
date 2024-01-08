#include "../minishell.h"

void set_oldpw(t_minishell *data, char *cwd)
{

    char* arg[2];

    arg[0] = "export";;
    arg[1] = strcat("OLDPWD=", cwd);
    ft_export(data, &arg[1]);
    free(arg[1]);
}

int ft_cd(char *token, t_minishell *data)
{
    (void) data;
    char cwd[PATH_MAX];

    if (token == NULL || token[0] == '\0')
    {
        char *home_dir = getenv("HOME");

        if (home_dir == NULL)
        {
            fprintf(stderr, "Home directory not found.\n");
            return 0;
        }

        //set_oldpw(data, getcwd(NULL, 0));

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
    else if (strcmp(token, "..") == 0)
    {
        //printf("%s", getcwd(cwd, sizeof(cwd)));
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd");
            return 0;
        }


        set_oldpw(data, getcwd(cwd, sizeof(cwd)));

        if (strcmp(cwd, "/") == 0)
        {
            printf("Already at the root directory.\n");
            return 1;
        }

        if (chdir("..") == 0)
        {
            set_oldpw(data, cwd);
            return 1;
        }
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

        //set_oldpw(data, cwd);

        if (chdir(token) == 0)
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

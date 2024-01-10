#include "../minishell.h"

int syntax_env_var(char **args)
{
    int i = -1;
    while (args[1][++i] && args[1][i] != '=')
        continue;
    if (args[1][i] == '=' && args[1][i + 1])
        return i;
    return -1;
}

void ft_export(t_minishell *data, char **args)
{
    if (args[1])
    {
        if (syntax_env_var(args) != -1)
        {
        int env_count = 0;

        while (data->env[env_count] != NULL)
        {
            if (strncmp(data->env[env_count], args[1], syntax_env_var(args)) == 0 &&
                data->env[env_count][syntax_env_var(args)] == '=')
            {
                free(data->env[env_count]);
                data->env[env_count] = strdup(args[1]);
                return;
            }
            env_count++;
        }

        char **new_env = (char **)malloc((env_count + 2) * sizeof(char *));
        int i = 0;

        while (i < env_count)
        {
            new_env[i] = strdup(data->env[i]);
            i++;
        }

        new_env[env_count] = strdup(args[1]);
        new_env[env_count + 1] = NULL;

        i = 0;
        while (i < env_count)
        {
            free(data->env[i]);
            i++;
        }
        free(data->env);

        data->env = new_env;
        }
    }
    else
    {
        int i = 0;
        if (data && data->env)
        {
            while (data->env[i] != NULL)
            {
                printf("declare -x %s\n", data->env[i]);
                ++i;
            }
        }
    
    }
}

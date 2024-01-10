#include "../minishell.h"

unsigned long int verif_env_var(char *env_var)
{
    int i = -1;
    while (env_var[++i] && env_var[i] != '=');

    if (env_var[i] == '=' && env_var[i + 1])
    {
        return (i);
    }
    else
        return 0;
}

void ft_unset(t_minishell *data, char **args)
{
    if (args[1])
    {
        int env_count = 0;
        size_t arg_len = strlen(args[1]);

        while (data->env[env_count] != NULL)
        {
            if (strncmp(data->env[env_count], args[1], verif_env_var(data->env[env_count])) == 0 &&
                verif_env_var(data->env[env_count]) == arg_len)
            {
                free(data->env[env_count]);

                // Shift the remaining elements
                while (data->env[env_count + 1] != NULL)
                {
                    data->env[env_count] = data->env[env_count + 1];
                    env_count++;
                }

                // Set the last element to NULL
                data->env[env_count] = NULL;

                return;
            }
            env_count++;
        }
    }
}

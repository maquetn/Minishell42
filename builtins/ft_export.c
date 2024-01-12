#include "../minishell.h"

int syntax_env_var(char **args)
{
    int i = -1;
    while (args[1][++i] && args[1][i] != '=')
    {
        if (isdigit(args[1][0]) || (!isalnum(args[1][i]) && args[1][i] != '_' && (args[1][i] == '+' && (i == 0 || args[1][i + 1] != '='))))
            return -1;
    }
    if (args[1][i] == '=')
        return i;
    return -1;
}

void ft_export(t_minishell *data, char **args)
{
    int exp = 1;
    //int plus_flag = 0;
    while (args[exp])
    {
        int env_count = 0;

        if (strcmp(args[1], "OLD_PWD=") == 0)
        {
            free(data->env[env_count]);
            data->env[env_count] = strdup(args[2]);
            printf("%s\n\n", data->env[env_count]);
            return;
        }
        if (syntax_env_var(args) != -1)
        {
            while (data->env[env_count] != NULL)
            {
                if (strncmp(data->env[env_count], args[1], syntax_env_var(args)) == 0 &&
                    data->env[env_count][syntax_env_var(args)] == '=')
                {
                    if(data->env[env_count][syntax_env_var(args) - 1] == '+')
                    {
                        //APPEND
                        printf("%s\n%s\n",data->env[env_count], args[1]);
                        strncat(data->env[env_count], args[1] + syntax_env_var(args)+1, strlen(args[1]) - syntax_env_var(args));
                    }
                    else
                    {
                        free(data->env[env_count]);
                        data->env[env_count] = strdup(args[1]);
                    }
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
            printf("\n\nici\n\n");
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

        exp++;
    }

    int i = 0;
    if (data && data->env && !args[1])
    {
        while (data->env[i] != NULL)
        {
            printf("declare -x %s\n", data->env[i]);
            ++i;
        }
    }
}

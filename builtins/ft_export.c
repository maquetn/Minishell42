#include "../minishell.h"

int syntax_env_var(char *arg)
{
    int i = 0;
    if(isdigit(arg[0]) != 0)
        return(-1);
    while (arg[i] && arg[i] != '=')
    {
        if (!isalnum(arg[i]) && arg[i] != '_')
        {
            if (arg[i] == '+' && arg[i + 1] != '=')
                return -1;
        }
        i++;
    }
    if (arg[i] == '=')
        return i;
    else
        return -1;
}

void ft_export(t_minishell *data, char **args)
{
    int exp = 1;
    while (args[exp])
    {
        char *current_arg = args[exp];
        int env_count = 0;

        while (data->env[env_count] != NULL)
        {
            if (strncmp(data->env[env_count], current_arg, syntax_env_var(current_arg)) == 0 &&
                data->env[env_count][syntax_env_var(current_arg)] == '=')
            {
                if (data->env[env_count][syntax_env_var(current_arg) - 1] == '+')
                {
                    // Append
                    size_t current_length = strlen(data->env[env_count]);
                    size_t append_length = strlen(current_arg + syntax_env_var(current_arg) + 1);

                    char *new_buffer = (char *)malloc(current_length + append_length + 1);
                    strcpy(new_buffer, data->env[env_count]);
                    strcat(new_buffer, current_arg + syntax_env_var(current_arg) + 1);
                    free(data->env[env_count]);
                    data->env[env_count] = new_buffer;
                }
                else
                {
                    // Update
                    free(data->env[env_count]);
                    data->env[env_count] = strdup(current_arg);
                }

                break;
            }
            env_count++;
        }

        if (data->env[env_count] == NULL)
        {
            // Variable not found, add it to env
            char *charPosition = strchr(current_arg, '+');

            char removed_plus[PATH_MAX];
            if (charPosition != NULL)
            {
                size_t removed_plus_length = charPosition - current_arg;
                strncpy(removed_plus, current_arg, removed_plus_length);
                removed_plus[removed_plus_length] = '\0';
                strcat(removed_plus, charPosition + 1);
            }

            char **new_env = (char **)malloc((env_count + 2) * sizeof(char *));
            int i = 0;

            while (i < env_count)
            {
                new_env[i] = strdup(data->env[i]);
                i++;
            }

            if (charPosition != NULL && strncmp(removed_plus, current_arg, charPosition - current_arg) == 0)
            {
                i = 0;
                int variableExists = 0;  // Flag to check if the variable already exists

                while (new_env[i])
                {
                    char *equalSignPos = strchr(new_env[i], '=');
                    if (equalSignPos != NULL)
                    {
                        size_t substringLength = equalSignPos - new_env[i];
                        char *substring = malloc((substringLength + 1) * sizeof(char));

                        size_t current_length = strlen(new_env[i]);
                        size_t append_length = strlen(strchr(removed_plus, '=') + 1);
                        size_t total_length = current_length + append_length;

                        new_env[i] = realloc(new_env[i], total_length + 1);

                        strncpy(substring, new_env[i], substringLength);
                        substring[substringLength] = '\0';

                        int biggest = 0;
                        if (substringLength > 0 && (substringLength > (strlen(removed_plus) - strlen(strchr(removed_plus, '=')))))
                            biggest = substringLength;
                        else if (strlen(removed_plus) > 0)
                            biggest = strlen(removed_plus) - strlen(strchr(removed_plus, '='));

                        if (strncmp(substring, removed_plus, biggest) == 0)
                        {
                            variableExists = 1;
                            strcat(new_env[i], strchr(removed_plus, '=') + 1);
                        }
                        free(substring);
                    }
                    i++;
                }

                if (!variableExists)
                {
                    char *charPosition = strchr(current_arg, '+');
                    size_t removed_plus_length = charPosition - current_arg;
                    strncpy(removed_plus, current_arg, removed_plus_length);
                    removed_plus[removed_plus_length] = '\0';
                    strcat(removed_plus, charPosition + 1);
                    new_env[i] = strdup(removed_plus);
                    new_env[i + 1] = NULL;
                }
            }

            else if(syntax_env_var(current_arg) != -1)
            {
                new_env[env_count] = strdup(current_arg);
                new_env[env_count + 1] = NULL;
            }
            printf("\n%d\n", syntax_env_var(current_arg));
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

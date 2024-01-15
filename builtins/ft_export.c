#include "../minishell.h"

int syntax_env_var(char *arg) {
    int i = 0;
    if (isdigit(arg[0]) != 0) {
        printf("Minishell: export: `%s': not a valid identifier\n", arg);
        return -1;
    }
    while (arg[i] && arg[i] != '=') {
        if (isalnum(arg[i]) == 0 && arg[i] != '_') {
            if (arg[i] != '+' || (arg[i] == '+' && arg[i + 1] != '=')) {
                printf("Minishell: export: `%s': not a valid identifier\n", arg);
                return -1;
            }
        }
        i++;
    }
    if (arg[i] == '=') {
        return i;
    } else {
        printf("Minishell: export: `%s': not a valid identifier\n", arg);
        return -1;
    }
}

void update_variable(char **new_env, char *removed_plus) {
    int i = 0, variable_exists = 0;

    while (new_env[i]) {
        char *equal_sign_pos = strchr(new_env[i], '=');

        if (equal_sign_pos != NULL) {
            size_t substring_length = equal_sign_pos - new_env[i];
            char *substring = malloc((substring_length + 1) * sizeof(char));

            size_t current_length = strlen(new_env[i]);
            size_t append_length = strlen(strchr(removed_plus, '=') + 1);
            size_t total_length = current_length + append_length;

            new_env[i] = realloc(new_env[i], total_length + 1);
            strncpy(substring, new_env[i], substring_length);
            substring[substring_length] = '\0';

            int biggest = (substring_length > 0 && (substring_length > (strlen(removed_plus) - strlen(strchr(removed_plus, '='))))) ?
                          substring_length : strlen(removed_plus) - strlen(strchr(removed_plus, '='));

            if (strncmp(substring, removed_plus, biggest) == 0) {
                variable_exists = 1;
                strcat(new_env[i], strchr(removed_plus, '=') + 1);
            }
            free(substring);
        }
        i++;
    }

    if (!variable_exists) {
        new_env[i] = strdup(removed_plus);
        new_env[i + 1] = NULL;
    }
}

void add_variable(t_minishell *data, char *removed_plus) {
    int env_count = 0;
    while (data->env[env_count] != NULL) {
        env_count++;
    }

    char **new_env = malloc((env_count + 2) * sizeof(char *));
    int i = 0;
    while (i < env_count) {
        new_env[i] = strdup(data->env[i]);
        i++;
    }

    // If `+=`, just add the variable without checking existing ones
    new_env[i] = strdup(removed_plus);
    new_env[i + 1] = NULL;

    i = 0;
    while (i < env_count) {
        free(data->env[i]);
        i++;
    }
    free(data->env);
    data->env = new_env;
}

void update_existing(t_minishell *data, char *current_arg, int env_count) {
    if ((strchr(current_arg, '+') != NULL && syntax_env_var(current_arg) != -1)) {
        // Append
        size_t current_length = strlen(data->env[env_count]);
        size_t append_length = strlen(current_arg + syntax_env_var(current_arg) + 1);

        char *new_buffer = malloc(current_length + append_length + 1);
        strcpy(new_buffer, data->env[env_count]);
        strcat(new_buffer, current_arg + syntax_env_var(current_arg) + 1);

        free(data->env[env_count]);
        data->env[env_count] = new_buffer;
    } else {
        // Update
        free(data->env[env_count]);
        data->env[env_count] = strdup(current_arg);
    }
}

void ft_export(t_minishell *data, char **args) {
    int exp = 1;
    while (args[exp]) {
        char *current_arg = args[exp];
        int env_count = 0;
        int append_flag = 0;

        while (data->env[env_count] != NULL) {
            if (syntax_env_var(current_arg) == -1) {
                break;  // Break if syntax is invalid
            }

            if (strncmp(data->env[env_count], current_arg, syntax_env_var(current_arg)) == 0 &&
                data->env[env_count][syntax_env_var(current_arg)] == '=') {
                update_existing(data, current_arg, env_count);
                break;
            }
            env_count++;
        }

        if (data->env[env_count] == NULL && syntax_env_var(current_arg) != -1) {
            char *char_position = strchr(current_arg, '+');
            char removed_plus[PATH_MAX];

            if (char_position != NULL) {
                size_t removed_plus_length = char_position - current_arg;
                strncpy(removed_plus, current_arg, removed_plus_length);
                removed_plus[removed_plus_length] = '\0';
                strcat(removed_plus, char_position + 1);
            } else {
                // No '+' found, set removed_plus as the entire current_arg
                strncpy(removed_plus, current_arg, PATH_MAX);
                removed_plus[PATH_MAX - 1] = '\0';
            }

            env_count = 0;
            while (data->env[env_count] != NULL) {
                if (syntax_env_var(removed_plus) == -1) {
                    break;  // Break if syntax is invalid
                }

                if (strncmp(data->env[env_count], removed_plus, syntax_env_var(removed_plus)) == 0 &&
                    data->env[env_count][syntax_env_var(removed_plus)] == '=') {
                    update_existing(data, current_arg, env_count);
                    append_flag = 1;
                    break;
                }
                env_count++;
            }

            if (syntax_env_var(removed_plus) != -1) {
                env_count++;
                if (append_flag == 0) {
                    add_variable(data, removed_plus);
                }
            }
        }
        exp++;
    }

    int i = 0;
    if (data && data->env && !args[1]) {
        while (data->env[i] != NULL) {
            printf("declare -x %s\n", data->env[i]);
            ++i;
        }
    }
}

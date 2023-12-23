#include "minishell.h"

int ft_export(Environment *env, const char *name, const char *value)
{
    // Check if the variable already exists
    for (size_t i = 0; i < env->size; ++i) {
        if (strcmp(env->vars[i].name, name) == 0)
        {
            // Variable already exists, update its value
            free(env->vars[i].value);
            env->vars[i].value = strdup(value);
            if (env->vars[i].value == NULL)
            {
                perror("Error duplicating environment variable value");
                return 1;
            }
            return 0; // Successfully updated existing variable
        }
    }

    // Variable doesn't exist, create a new one
    EnvVar new_var;

    // Check if the name includes an equal sign
    const char *equal_sign = strchr(name, '=');
    if (equal_sign != NULL) {
        // Split the name and value
        size_t name_length = equal_sign - name;
        new_var.name = strndup(name, name_length);
        new_var.value = strdup(equal_sign + 1);
    } else {
        // No equal sign, use the provided name and value
        new_var.name = strdup(name);
        new_var.value = strdup(value);
    }

    if (new_var.name == NULL || new_var.value == NULL)
    {
        perror("Error duplicating environment variable data");
        return 1;
    }

    // Resize the environment variables array
    env->vars = realloc(env->vars, (env->size + 1) * sizeof(EnvVar));
    if (env->vars == NULL)
    {
        perror("Error reallocating environment variables array");
        return 1;
    }

    // Add the new variable to the array
    env->vars[env->size] = new_var;
    env->size++;

    return 0; // Successfully added new variable
}
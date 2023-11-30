#include "minishell.h"

char *custom_realpath(const char *path, char *resolved_path)
{
    char temp[PATH_MAX];
    char *token, *ptr, *lastToken = NULL;

    if (path[0] == '/')
    {
        // If the path is absolute, start with the root directory
        strcpy(temp, "/");
    }
    else
    {
        // If the path is relative, start with the current working directory
        if (getcwd(temp, sizeof(temp)) == NULL)
        {
            perror("getcwd");
            exit(EXIT_FAILURE);
        }
        strcat(temp, "/");
    }

    strcat(temp, path);

    token = strtok_r(temp, "/", &ptr);

    while (token != NULL)
    {
        if (strcmp(token, ".") == 0)
        {
            // Ignore "." (current directory)
        } else if (strcmp(token, "..") == 0)
        {
            // Go up one directory for ".."
            if (lastToken != NULL)
            {
                lastToken = strrchr(resolved_path, '/');
                if (lastToken != NULL)
                {
                    *lastToken = '\0';
                }
            }
        } else
        {
            // Add other components to the resolved path
            strcat(resolved_path, "/");
            strcat(resolved_path, token);
            lastToken = resolved_path + strlen(resolved_path);
        }

        token = strtok_r(NULL, "/", &ptr);
    }

    if (strlen(resolved_path) == 0)
    {
        // If resolved path is empty, set it to the root directory
        strcpy(resolved_path, "/");
    }

    return resolved_path;
}

char *expand_path(char *input)
{
    // Check if the path is already absolute
    if (input[0] == '/')
    {
        return strdup(input);  // Return a duplicate of the input
    }

    // Use the custom realpath function
    char resolved_path[PATH_MAX];
    custom_realpath(input, resolved_path);

    return strdup(resolved_path);  // Return a duplicate of the resolved path
}


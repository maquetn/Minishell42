#include "minishell.h"

void expand_path_in_tokens(char **input)
{
    char *expanded_path = expand_path(*input);
    *input = expanded_path;
}

void trim(char *str)
{
    int start = 0, end = ft_strlen(str) - 1;

    while (str[start] == ' ')
        start++;
    while (end > start && str[end] == ' ')
        end--;
    for (int i = 0; i <= end - start; ++i)
        str[i] = str[start + i];
    str[end - start + 1] = '\0';
}

void token(char *input)
{
    t_token *tokens[100] = {NULL};
    int arg = 0;
    int i = 0;
    int j = 0;

    while (input[i])
    {
        if (input[i] == '|' || input[i] == '>' || input[i] == '<' || input[i] == ' ' || input[i] == '\0')
        {
            if (j > 0)
            {
                tokens[arg] = (t_token *)malloc(sizeof(t_token));
                if (tokens[arg] == NULL)
                {
                    perror("malloc failed");
                    ft_exit(EXIT_FAILURE);
                }

                tokens[arg]->content = (char *)malloc(j + 1);
                if (tokens[arg]->content == NULL)
                {
                    perror("malloc failed");
                    ft_exit(EXIT_FAILURE);
                }

                strncpy(tokens[arg]->content, input + i - j, j);
                tokens[arg]->content[j] = '\0';

                trim(tokens[arg]->content);
                check_type(tokens, arg);

                arg++;
                j = 0;
            }

            if (input[i] != ' ')
            {
                tokens[arg] = (t_token *)malloc(sizeof(t_token));
                if (tokens[arg] == NULL)
                {
                    perror("malloc failed");
                    ft_exit(EXIT_FAILURE);
                }

                if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
                {
                    tokens[arg]->content = malloc(3);
                    if (tokens[arg]->content == NULL)
                    {
                        perror("malloc failed");
                        ft_exit(EXIT_FAILURE);
                    }

                    tokens[arg]->content[0] = input[i];
                    tokens[arg]->content[1] = input[i + 1];
                    tokens[arg]->content[2] = '\0';
                    i++;
                }
                else
                {
                    tokens[arg]->content = malloc(2);
                    if (tokens[arg]->content == NULL)
                    {
                        perror("malloc failed");
                        ft_exit(EXIT_FAILURE);
                    }

                    tokens[arg]->content[0] = input[i];
                    tokens[arg]->content[1] = '\0';
                }
                check_type(tokens, arg);
                arg++;
            }
        }
        else
        {
            j++;
        }

        i++;
    }

    if (j > 0)
    {
        tokens[arg] = (t_token *)malloc(sizeof(t_token));
        if (tokens[arg] == NULL)
        {
            perror("malloc failed");
            ft_exit(EXIT_FAILURE);
        }

        tokens[arg]->content = strdup(input + i - j);
        if (tokens[arg]->content == NULL)
        {
            perror("strdup failed");
            ft_exit(EXIT_FAILURE);
        }

        trim(tokens[arg]->content);
        check_type(tokens, arg);
        if (tokens[arg]->type == CMD)
            exec_builtin(tokens, arg);
        arg++;
    }

    i = 0;
    while (tokens[i] != NULL)
    {
        //printf("Token %d: %s\n", i, tokens[i]->content);
        free(tokens[i]->content);
        tokens[i]->content = NULL;
        free(tokens[i]);
        tokens[i] = NULL;

        i++;
    }
}

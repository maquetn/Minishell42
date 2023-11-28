#include "minishell.h"

void trim(char *str)
{
    int start = 0, end = ft_strlen(str) - 1;

    while (str[start] == ' ')
    {
        start++;
    }

    while (end > start && str[end] == ' ')
    {
        end--;
    }

    for (int i = 0; i <= end - start; ++i)
    {
        str[i] = str[start + i];
    }

    str[end - start + 1] = '\0';
}

int token(char *input)
{
    t_token *tokens[100];
    int content = 0;
    int i = 0;
    int j = 0;

    while (input[i])
    {
        if (input[i] == '|' || input[i] == '>' || input[i] == '<' || input[i] == ' ' || input[i] == '\0')
        {
            if (j > 0)
            {
                tokens[content] = (t_token *)malloc(sizeof(t_token));
                tokens[content]->content = malloc(j + 1); // Allocate memory for the content
                strncpy(tokens[content]->content, input + i - j, j);
                tokens[content]->content[j] = '\0';
                trim(tokens[content]->content);
                check_type(tokens, content);
				printf("%d	", tokens[content]->type);
                printf("Token: %s\n", tokens[content]->content);
                content++;
                j = 0;
            }

            if (input[i] != ' ')
            {
                tokens[content] = (t_token *)malloc(sizeof(t_token));
                if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
                {
                    tokens[content]->content = malloc(3);
                    tokens[content]->content[0] = input[i];
                    tokens[content]->content[1] = input[i + 1];
                    tokens[content]->content[2] = '\0';
                    i++;
                }
                else
                {
                    tokens[content]->content = malloc(2);
                    tokens[content]->content[0] = input[i];
                    tokens[content]->content[1] = '\0';
                }
				check_type(tokens, content);
				printf("%d	", tokens[content]->type);
                printf("Delimiter: %s\n", tokens[content]->content);
                content++;
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
        tokens[content] = (t_token *)malloc(sizeof(t_token));
        tokens[content]->content = strndup(input + i - j, j);
        trim(tokens[content]->content);
		check_type(tokens, content);
        printf("%d	", tokens[content]->type);
        printf("Token: %s\n", tokens[content]->content);
        content++;
    }

    for (int i = 0; i < content; ++i)
    {
        free(tokens[i]->content);
        free(tokens[i]);
    }

    return 0;
}

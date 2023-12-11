#include "minishell.h"

void ft_echo(t_token **tokens, int index)
{
    while (tokens[index] != NULL)
    {
        printf("%s ", tokens[index]->content);
        index++;
    }
    printf("\n");
}

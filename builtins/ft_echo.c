#include "minishell.h"

void ft_echo(char **args, int index)
{
    while (args[index] != NULL)
    {
        printf("%s ", args[index]);
        index++;
    }
    printf("\n");
}

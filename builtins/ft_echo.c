#include "../minishell.h"

void ft_echo(char **args, int index)
{
    int n = 0;
    unsigned long i = 0;

    if (args[index] != NULL)
    {
        if (args[index][0] == '-')
        {
            i = 1;  // Start from the second character
            while (args[index][i] == 'n')
                i++;

            if (i == strlen(args[index]) && i > 1)
            {
                index++;
                n = 1;
            }
        }
    }

    while (args[index] != NULL)
    {
        printf("%s ", args[index]);
        index++;
    }

    if (n == 0)
        printf("\n");
}

//#include "minishell.h"
#include <string.h>
#include <unistd.h>

static int nb_args(char **args)
{
    int size = 0;
    while (args[size])
        size++;
    return size;
}

int ft_echo(char **args)
{
    int i;
    int n_option;

    i = 1;
    n_option = 0;
    if (nb_args(args) > 1)
    {
        while (args[i] && strcmp(args[i], "-n") == 0)
        {
            n_option = 1;
            i++;
        }
        while (args[i])
        {
            write(1, args[i], strlen(args[i]));
            if (args[i + 1] && args[i][0] != '\0')
                write(1, " ", 1);
            i++;
        }
    }
    if (n_option == 0)
        write(1, "\n", 1);
    return 0;
}

int main(int argc, char *argv[])
{
    ft_echo(argv);
}

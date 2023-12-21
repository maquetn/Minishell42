#include "minishell.h"

int ft_exit(int i)
{
    g_exit_code = i;
    exit(g_exit_code); //replace by exit code
}

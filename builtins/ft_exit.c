#include "minishell.h"

int ft_exit(int i, char **args)
{
	g_exit_code = i;
	if (args[1] && (atoi(args[1]) != 0))
		g_exit_code = atoi(args[1]);
	exit(g_exit_code); //replace by exit code
}

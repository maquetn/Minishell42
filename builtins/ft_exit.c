#include "minishell.h"

int ft_exit(int i, char **args)
{
	g_exit_code = i;
	if (args[1] && (atoi(args[1]) != 0))
		g_exit_code = atoi(args[1]);
	/*if (index > 1)
		printf("exit: too many arguments");
		//pas d exit
	if ((atoi(args[1]) == 0))
		printf("exit: test: numeric argument required");
		//exit quand meme*/
	//printf("%d", g_exit_code);
	exit(g_exit_code); //replace by exit code
}

#include "../minishell.h"

void ft_echo(char **args, int index)
{
	int n;
	unsigned long i;

	n = 0;
	i = 0;
	while (args[index] != NULL)
	{
		if (args[index][0] == '-')
		{
			i = 1;
			while (args[index][i] == 'n')
				i++;
			if (i == strlen(args[index]) && i > 1)
			{
				index++;
				n = 1;
			}
			else
				break;
		}
		else
			break;
	}
	while (args[index] != NULL)
	{
		printf("%s ", args[index]);
		index++;
	}
	if (n == 0)
		printf("\n");
}

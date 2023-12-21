#include "minishell.h"

void ft_env(t_minishell *data)
{
	if (data == NULL || data->env == NULL)
	{
		fprintf(stderr, "Error: Invalid data or environment.\n");
		return;
	}

	printf("Environment:\n");

	int i = 0;
	while (data->env[i] != NULL)
	{
		printf("%s\n", data->env[i]);
		++i;
	}
}
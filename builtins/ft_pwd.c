#include "minishell.h"

char *ft_pwd()
{
	char *cwd = malloc(PATH_MAX);
	if (cwd == NULL)
	{
		perror("malloc");
		ft_exit(EXIT_FAILURE, NULL);
	}

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("getcwd");
		free(cwd);
		ft_exit(EXIT_FAILURE, NULL);
	}

	printf("%s\n", cwd);
	free(cwd);

	return NULL;
}
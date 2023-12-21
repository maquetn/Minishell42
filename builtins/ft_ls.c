#include "minishell.h"

void ft_ls(const char *directory_path)
{
	DIR *dir;
	struct dirent *entry;

	dir = opendir(directory_path);

	if (dir == NULL)
	{
		perror("opendir");
		ft_exit(EXIT_FAILURE);
	}

	while ((entry = readdir(dir)) != NULL)
	{
		printf("%s\n", entry->d_name);
	}

	closedir(dir);
}

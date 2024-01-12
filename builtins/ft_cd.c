#include "../minishell.h"


void	oldpwd(t_minishell *data, char *cwd)
{
	char *old_pwd;

	cwd = getcwd(NULL, 0);
	old_pwd = ft_strjoin("OLDPWD=", cwd, data);

	char **tab;

	tab = gc_malloc(sizeof(char * ) * 3, data);
	tab[0] = ft_strdup("export", data);
	tab[1] = ft_strdup(old_pwd, data);
	tab[2] = NULL;

    ft_export(data, tab);

}

int ft_cd(t_minishell *data, char *token)
{
    char cwd[PATH_MAX];

	oldpwd(data, NULL);

	if(token[1])
	{
		printf("cd: string not in pwd: ..\n");
		return(1);
	}
    if (token == NULL || token[0] == '\0' || (strcmp(&token[0], "~") == 0))
    {
        char *home_dir = getenv("HOME");

        if (home_dir == NULL)
        {
            fprintf(stderr, "Home directory not found.\n");
            return 0;
        }

        if (chdir(home_dir) == 0)
            return 1;
        else
        {
            perror("cd");
            return 0;
        }
    }
    else if (strcmp(token, "..") == 0)
	{
		getcwd(cwd, sizeof(cwd));
		if ((strcmp(cwd, "/Users")) == 0)
		{
			chdir("/");
			return(1);
		}
		if (chdir("..") == 0)
			return 1;
		else
		{
			perror("cd");
			return 0;
		}
	}
    else if (strcmp(token, "-") == 0)
	{
		if (chdir(getenv("OLDPWD")) == 0)
			return 1;
		else
		{
			perror("cd");
			return 0;
		}
	}
    else
    {
		char *home_dir = getenv("HOME");
		if(strncmp(token, "~", 1) == 0)
		{
			token = token + 1;
			if(strcmp(token, "/") == 0)
				token = home_dir;
			chdir(home_dir);
		}
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd");
            return 0;
        }

        if (chdir(token) == 0)
        {
            char *target_path = getcwd(NULL, 0);
            if (target_path == NULL)
            {
                perror("getcwd");
                chdir(cwd);
                return 0;
            }
            chdir(target_path);
            free(target_path);

            return 1;
        }
        else
        {
            perror("cd");
            return 0;
        }
    }
}

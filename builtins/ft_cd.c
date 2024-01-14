/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 14:42:58 by mdor              #+#    #+#             */
/*   Updated: 2024/01/14 14:43:00 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	oldpwd(t_minishell *data, char *cwd)
{
	char	*old_pwd;
	char	**tab;

	cwd = getcwd(NULL, 0);
	old_pwd = ft_strjoin("OLDPWD=", cwd, data);
	tab = gc_malloc(sizeof(char *) * 3, data);
	tab[0] = ft_strdup("export", data);
	tab[1] = ft_strdup(old_pwd, data);
	tab[2] = NULL;
	ft_export(data, tab);
}

// int change_to_home_dir(t_minishell *data)
// {
//     char *home_dir = getenv("HOME");
//     if (home_dir == NULL)
// 	{
//         fprintf(stderr, "Home directory not found.\n");
//         return (0);
//     }
//     return (chdir(home_dir) == 0 ? 1 : (perror("cd"), 0));
// }

// int change_to_parent_dir(void)
// {
//     char cwd[PATH_MAX];
//     getcwd(cwd, sizeof(cwd));
//     if (strcmp(cwd, "/Users") == 0)
//         return (chdir("/") == 0 ? 1 : (perror("cd"), 0));
//     return (chdir("..") == 0 ? 1 : (perror("cd"), 0));
// }

// int change_to_oldpwd(void)
// {
//     return (chdir(getenv("OLDPWD")) == 0 ? 1 : (perror("cd"), 0));
// }

// int change_to_custom_dir(char *token)
// {
//     char cwd[PATH_MAX];
//     char *home_dir = getenv("HOME");
//     if (strncmp(token, "~", 1) == 0)
// 	{
//         token = token + 1;
//         if (strcmp(token, "/") == 0)
//             token = home_dir;
//         chdir(home_dir);
//     }
//     if (getcwd(cwd, sizeof(cwd)) == NULL)
//         return ((perror("getcwd"), 0));
//     if (chdir(token) == 0)
// 	{
//         char *target_path = getcwd(NULL, 0);
//         if (target_path == NULL)
//             return (perror("getcwd"), chdir(cwd), 0);
//         int result = chdir(target_path);
//         free(target_path);
//         return (result == 0 ? 1 : (perror("cd"), 0));
//     }
// 	else
// 	{
//         perror("cd");
//         return (0);
//     }
// }

// int ft_cd(t_minishell *data, char *token)
// {
//     oldpwd(data, NULL);
//     if (token == NULL || token[0] == '\0' || (ft_strcmp(&token[0], "~", data) == 0))
//         return (change_to_home_dir(data));
//     else if (strcmp(token, "..") == 0)
//         return (change_to_parent_dir());
//     else if (strcmp(token, "-") == 0)
//         return (change_to_oldpwd());
//     else
//         return (change_to_custom_dir(token));
// }

int	ft_cd(t_minishell *data, char *token)
{
	char	cwd[PATH_MAX];
	char	*home_dir;

	oldpwd(data, NULL);
	if (token == NULL || token[0] == '\0' || (ft_strcmp(&token[0], "~", data) == 0))
	{
		home_dir = getenv("HOME");
		if (home_dir == NULL)
		{
			fprintf(stderr, "Home directory not found.\n");
			return (0);
		}
		if (chdir(home_dir) == 0)
			return (1);
		else
		{
			perror("cd");
			return (0);
		}
	}
	else if (strcmp(token, "..") == 0)
	{
		getcwd(cwd, sizeof(cwd));
		if ((strcmp(cwd, "/Users")) == 0)
		{
			chdir("/");
			return (1);
		}
		if (chdir("..") == 0)
			return (1);
		else
		{
			perror("cd");
			return (0);
		}
	}
	else if (strcmp(token, "-") == 0)
	{
		if (chdir(getenv("OLDPWD")) == 0)
			return (1);
		else
		{
			perror("cd");
			return (0);
		}
	}
	else
	{
		char *home_dir = getenv("HOME");
		if (strncmp(token, "~", 1) == 0)
		{
			token = token + 1;
			if (strcmp(token, "/") == 0)
				token = home_dir;
			chdir(home_dir);
		}
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd");
			return (0);
		}

		if (chdir(token) == 0)
		{
			char *target_path = getcwd(NULL, 0);
			if (target_path == NULL)
			{
				perror("getcwd");
				chdir(cwd);
				return (0);
			}
			chdir(target_path);
			free(target_path);
			return (1);
		}
		else
		{
			perror("cd");
			return (0);
		}
	}
}

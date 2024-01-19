/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::		  */
/*   ft_cd.c											:+:	  :+:	:+:		  */
/*													+:+ +:+		 +:+		  */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+			  */
/*   Created: 2024/01/14 14:42:58 by mdor			  #+#	#+#				  */
/*   Updated: 2024/01/14 14:43:00 by mdor			 ###   ########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

int	change_to_home_dir(t_minishell *data)
{
	char	*home_dir;

	home_dir = get_env("HOME", data->env, data);
	if (home_dir == NULL)
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		return (1);
	}
	if (chdir(home_dir) == 0)
		return (0);
	else
	{
		perror("cd");
		return (1);
	}
}

int	change_to_parent_dir(t_minishell *data)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		printf("Error : Trying to reach an unknown \
		directory, going to Home instead\n");
		return (change_to_home_dir(data));
	}
	if (ft_strcmp(cwd, "/Users", data) == 0)
	{
		if (chdir("/") == 0)
			return (1);
		else
		{
			perror("cd");
			return (0);
		}
	}
	if (chdir("..") == 0)
		return (1);
	else
	{
		perror("cd");
		return (0);
	}
}

int	change_to_oldpwd(t_minishell *data)
{
	char	*env_path;

	env_path = get_env("OLDPWD", data->env, data);
	if (chdir(env_path) == 0)
	{
		printf("%s\n", env_path);
		return (0);
	}
	else
	{
		fprintf(stderr, "minishell: cd: OLDPWD not set\n");
		return (1);
	}
}

int	change_to_custom_dir(char *token, t_minishell *data)
{
	char	*home_dir;

	home_dir = getenv("HOME");
	if (!home_dir)
	{
		perror("getenv: HOME not set");
		return (1);
	}
	if (ft_strncmp(token, "~", 1) == 0)
	{
		if (ft_strcmp(token + 1, "/", data) == 0)
			token = home_dir;
		else
			token = ft_strjoin(home_dir, token + 1, data);
	}
	if (chdir(token) != 0)
	{
		perror("chdir");
		return (1);
	}
	return (0);
}

int	ft_cd(t_minishell *data, char *token)
{
	char	**oldpwd_toexport;
	char	**new_pwd;
	int		error;

	oldpwd_toexport = oldpwd(data);
	if (token == NULL || token[0] == '\0'
		|| (ft_strcmp(&token[0], "~", data) == 0))
		error = change_to_home_dir(data);
	else if (ft_strcmp(token, "..", data) == 0)
		error = change_to_parent_dir(data);
	else if (ft_strcmp(token, "-", data) == 0)
		error = change_to_oldpwd(data);
	else
		error = change_to_custom_dir(token, data);
	ft_export(data, oldpwd_toexport);
	new_pwd = newpwd(data);
	ft_export(data, new_pwd);
	data->error_trigger = error;
	return (0);
}

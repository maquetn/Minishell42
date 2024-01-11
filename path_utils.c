/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:37:18 by mdor              #+#    #+#             */
/*   Updated: 2024/01/11 16:37:20 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env(char *name, char **env, t_minishell *data)
{
	char	*path;
	int		i;
	int		j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		path = ft_substr(env[i], 0, j, data);
		if (strcmp(path, name) == 0)
		{
			return (env[i] + j + 1);
		}
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env, t_minishell *data)
{
	char	*executable;
	int		i;
	char	**paths;
	char	*potential_path;

	i = -1;
	paths = ft_split(get_env("PATH", env, data), ':', data);
	if (!paths)
	{
		return (ft_strdup(cmd, data));
	}
	while (paths[++i])
	{
		potential_path = ft_strjoin(paths[i], "/", data);
		executable = ft_strjoin(potential_path, cmd, data);
		if (access(executable, F_OK | X_OK) == 0)
		{
			return (executable);
		}
	}
	return (ft_strdup(cmd, data));
}

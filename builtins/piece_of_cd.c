/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piece_of_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 02:45:32 by mdor              #+#    #+#             */
/*   Updated: 2024/01/18 17:47:56 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**oldpwd(t_minishell *data)
{
	char	*old_pwd;
	char	**tab;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if(cwd == NULL)
		return (NULL);
	old_pwd = ft_strjoin("OLDPWD=", cwd, data);
	free(cwd);
	tab = gc_malloc(sizeof(char *) * 3, data);
	tab[0] = ft_strdup("export", data);
	tab[1] = ft_strdup(old_pwd, data);
	tab[2] = NULL;
	return (tab);
}

char	**newpwd(t_minishell *data)
{
	char	*new_pwd;
	char	**tab;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if(cwd == NULL)
		return (NULL);
	new_pwd = ft_strjoin("PWD=", cwd, data);
	free(cwd);
	tab = gc_malloc(sizeof(char *) * 3, data);
	tab[0] = ft_strdup("export", data);
	tab[1] = ft_strdup(new_pwd, data);
	tab[2] = NULL;

	return (tab);
}

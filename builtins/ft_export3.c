/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:36:13 by nmaquet           #+#    #+#             */
/*   Updated: 2024/01/18 19:31:48 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	syntax_loop(char *arg, int i)
{
	if (arg[i] == '=')
		return (-1);
	while (arg[i] && arg[i] != '=')
	{
		if (isalnum(arg[i]) == 0 && arg[i] != '_')
		{
			if ((isalnum(arg[i]) == 0 && arg[i] != '+') || \
				(arg[i] == '+' && arg[i + 1] != '='))
				return (-1);
		}
		i++;
	}
	return (i);
}

int	syntax_env_var(char *arg)
{
	int	i;

	i = 0;
	if (isdigit(arg[0]) != 0)
	{
		printf("Minishell: export: `%s': not a valid identifier\n", arg);
		return (-1);
	}
	i = syntax_loop(arg, i);
	if (arg[i] == '=')
		return (i);
	else
	{
		i = 0;
		while (arg[i] && arg[i] != '=')
		{
			i++;
		}
		if (arg[i] == '=')
			printf("Minishell: export: `%s': not a valid identifier\n", arg);
		return (-1);
	}
}

char	**ft_existence(int i, char **new_env, char *removed_plus, int j)
{
	if (j == 0)
		ft_strcat(new_env[i], ft_strchr(removed_plus, '=') + 1);
	else if (j == 1)
	{
		new_env[i] = no_gc_strdup(removed_plus);
		new_env[i + 1] = NULL;
	}
	return (new_env);
}

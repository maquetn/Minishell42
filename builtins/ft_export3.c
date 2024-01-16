/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:36:13 by nmaquet           #+#    #+#             */
/*   Updated: 2024/01/16 13:49:31 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	syntax_loop(char *arg, int i)
{
	while (arg[i] && arg[i] != '=')
	{
		if (isalnum(arg[i]) == 0 && arg[i] != '_')
		{
			if (arg[i] == '+' && arg[i + 1] != '=')
			{
				printf("Minishell: export: `%s': \
                    not a valid identifier\n", arg);
				return (-1);
			}
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
		printf("Minishell: export: `%s': not a valid identifier\n", arg);
		return (-1);
	}
}

char	**ft_existence(int i, char **new_env, char *removed_plus, int j)
{
	if (j == 0)
		strcat(new_env[i], strchr(removed_plus, '=') + 1);
	else if (j == 1)
	{
		new_env[i] = strdup(removed_plus);
		new_env[i + 1] = NULL;
	}
	return (new_env);
}

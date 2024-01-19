/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 23:21:14 by mdor              #+#    #+#             */
/*   Updated: 2024/01/19 13:41:32 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_flags(char **args, int *index, int *n)
{
	int	i;

	*n = 0;
	while (args[*index] != NULL)
	{
		if (args[*index][0] == '-')
		{
			i = 1;
			while (args[*index][i] == 'n')
			{
				i++;
			}
			if (i == (ft_strlen(args[*index])) && i > 1)
			{
				(*index)++;
				*n = 1;
			}
			else
				break ;
		}
		else
			break ;
	}
}

void	print_echo(char **args, int index, int n)
{
	while (args[index] != NULL)
	{
		if (!args[index + 1])
			break ;
		printf("%s ", args[index]);
		index++;
	}
	if (args[index] != NULL)
		printf("%s", args[index]);
	if (n == 0)
	{
		printf("\n");
	}
}

void	ft_echo(char **args, int index)
{
	int	n;

	process_flags(args, &index, &n);
	print_echo(args, index, n);
}

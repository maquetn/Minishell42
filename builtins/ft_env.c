/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 14:12:16 by mdor              #+#    #+#             */
/*   Updated: 2024/01/14 14:12:18 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_env(t_minishell *data)
{
	int	i;

	i = 0;
	if (data == NULL || data->env == NULL)
	{
		fprintf(stderr, "Error: Invalid data or environment.\n");
		return ;
	}
	while (data->env[i] != NULL)
	{
		printf("%s\n", data->env[i]);
		++i;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 14:12:16 by mdor              #+#    #+#             */
/*   Updated: 2024/01/19 13:36:01 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_env(t_minishell *data)
{
	int	i;

	i = 0;
	if (data == NULL || data->env == NULL)
	{
		ft_putstr_fd("Error: Invalid data or environment.\n", 2);
		return ;
	}
	while (data->env[i] != NULL)
	{
		printf("%s\n", data->env[i]);
		++i;
	}
}

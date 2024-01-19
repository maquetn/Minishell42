/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 02:41:27 by mdor              #+#    #+#             */
/*   Updated: 2024/01/19 13:54:39 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_data(t_minishell *data)
{
	free(data->input);
	free_custom_alloc(data);
	data->code = data->error_trigger;
	data->error_trigger = 0;
	data->head = NULL;
	data->first_token = NULL;
	data->node = NULL;
}

void	routine(t_minishell *data)
{
	token(data->input, data);
	if (data->first_token && data->error_trigger == 0)
		expander(data);
	if (data->first_token && data->error_trigger == 0)
		planting(data);
	signal(SIGQUIT, sigquit);
	if (data->node && data->error_trigger == 0)
		execute_simple_cmd(data->node, data, NULL);
	reset_data(data);
}

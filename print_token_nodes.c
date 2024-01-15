/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token_nodes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 06:52:46 by mdor              #+#    #+#             */
/*   Updated: 2024/01/15 06:52:53 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *head)
{
	printf("these are the tokens : \n\n");
	while (head)
	{
		printf("type : %u content : %s\n", head->type, head->content);
		head = head->next;
	}
	printf("\nend of token display\n\n");
}

void	print_nodes(t_minishell *data)
{
	int	i;

	i = 0;
	while (data->node)
	{
		printf("path : %s\n", data->node->path_to_cmd);
		while (data->node->args[i])
		{
			printf("args %d : %s\n", i, data->node->args[i]);
			i++;
		}
		data->node = data->node->next;
	}
}

void	free_tabl(char **tabl)
{
	int	i;

	i = 0;
	while (tabl[i])
	{
		free(tabl[i]);
		i++;
	}
	free(tabl);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:26:49 by mdor              #+#    #+#             */
/*   Updated: 2024/01/11 16:26:51 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_file(char *name, t_minishell *data, t_files_list **head)
{
	t_files_list	*newnode;
	t_files_list	*temp;

	temp = NULL;
	newnode = gc_malloc(sizeof(t_files_list), data);
	newnode->name = ft_strdup(name, data);
	newnode->next = NULL;
	if (*head == NULL)
		*head = newnode;
	else
	{
		temp = *head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = newnode;
	}
}

void	init_simple_cmd(t_simple_cmd *cmd)
{
	cmd->path_to_cmd = NULL;
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->prev = NULL;
	cmd->next = NULL;
	cmd->append_mode = 0;
	cmd->heredoc = 0;
}

t_simple_cmd	*get_cmd(t_token *token, t_minishell *data)
{
	t_simple_cmd	*cmd;

	cmd = create_simple_cmd(data, token);
	return (cmd);
}

t_simple_cmd	*recursive_parsing(t_minishell *data)
{
	t_simple_cmd	*prev;
	t_simple_cmd	*next;

	next = NULL;
	prev = NULL;
	prev = get_cmd(data->first_token, data);
	if (data->first_token)
	{
		next = recursive_parsing(data);
		prev->next = next;
		next->prev = prev;
	}
	return (prev);
}

void	planting(t_minishell *data)
{
	data->node = recursive_parsing(data);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:10:11 by mdor              #+#    #+#             */
/*   Updated: 2024/01/11 17:10:12 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_pipe_token(t_minishell *data, t_token **token)
{
	if ((*token)->next == NULL || (*token)->next->type == PIPE)
		data->error_trigger = print_syntax_error(*token);
	*token = (*token)->next;
}

void	process_heredoc(t_minishell *data, t_token **token, t_simple_cmd *cmd)
{
	if ((*token)->next == NULL || 
		(*token)->next->type == PIPE || (*token)->next->type == HEREDOC
		|| (*token)->next->type == INPUT || (*token)->next->type == OUTPUT
		|| (*token)->next->type == APPEND)
	{
		data->error_trigger = print_syntax_error(*token);
		if ((*token)->next != NULL)
			*token = (*token)->next->next;
		else
			*token = (*token)->next;
		return ;
	}
	cmd->heredoc_string = manage_heredoc((*token)->next->content,
			data, (*token)->next->quoted_heredoc);
	*token = (*token)->next->next;
	cmd->heredoc = 1;
}

void	process_append(t_minishell *data, t_token **token, t_simple_cmd *cmd)
{
	if ((*token)->next == NULL || 
		(*token)->next->type == PIPE || (*token)->next->type == HEREDOC
		|| (*token)->next->type == INPUT || (*token)->next->type == OUTPUT
		|| (*token)->next->type == APPEND)
	{
		data->error_trigger = print_syntax_error(*token);
		if ((*token)->next != NULL)
			*token = (*token)->next->next;
		else
			*token = (*token)->next;
		return ;
	}
	if (add_file((*token)->next->content, data, &cmd->output))
	{
		data->error_trigger = 1;
		return ;
	}
	*token = (*token)->next->next;
	cmd->append_mode = 1;
	return ;
}

void	process_input(t_minishell *data, t_token **token, t_simple_cmd *cmd)
{
	if ((*token)->next == NULL || 
		(*token)->next->type == PIPE || (*token)->next->type == APPEND
		|| (*token)->next->type == OUTPUT)
	{
		data->error_trigger = print_syntax_error(*token);
		if ((*token)->next != NULL)
			*token = (*token)->next->next;
		else
			*token = (*token)->next;
		return ;
	}
	if (add_file((*token)->next->content, data, &cmd->input))
	{
		data->error_trigger = 1;
		return ;
	}
	*token = (*token)->next->next;
	cmd->heredoc = 0;
	return ;
}

void	process_output(t_minishell *data, t_token **token, t_simple_cmd *cmd)
{
	if ((*token)->next == NULL || 
		(*token)->next->type == PIPE || (*token)->next->type == INPUT
		|| (*token)->next->type == HEREDOC)
	{
		data->error_trigger = print_syntax_error(*token);
		if ((*token)->next != NULL)
			*token = (*token)->next->next;
		else
			*token = (*token)->next;
		return ;
	}
	if (add_file((*token)->next->content, data, &cmd->output))
	{
		data->error_trigger = 1;
		return ;
	}
	*token = (*token)->next->next;
	cmd->append_mode = 0;
	return ;
}

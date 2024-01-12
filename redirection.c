/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 21:29:48 by mdor              #+#    #+#             */
/*   Updated: 2024/01/12 21:29:49 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_input(t_simple_cmd *cmd, int *p_fd, t_minishell *data)
{
	int	input_fd;

	input_fd = 0;
	if (cmd->heredoc == 1)
	{
		input_fd = open_all(cmd);
		if (input_fd == -1 || cmd->heredoc_string == NULL)
			exit(EXIT_FAILURE);
		input_fd = redirect_heredoc(cmd, data);
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	else if (cmd->input != NULL) 
	{
		input_fd = open_all(cmd);
		if (input_fd == -1) 
			exit(EXIT_FAILURE);
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	else if (cmd->prev != NULL && p_fd)
	{
		dup2(p_fd[0], 0);
		close(p_fd[1]);
	}
}

void	redirect_output(t_simple_cmd *cmd, int *p_fd)
{
	int	output_fd;

	output_fd = 1;
	if (cmd->output != NULL) 
	{
		output_fd = create_all_open_last(cmd);
		if (output_fd == -1) 
		{
			exit(EXIT_FAILURE);
		}
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	else if (cmd->next != NULL && p_fd)
	{
		dup2(p_fd[1], 1);
		close(p_fd[0]);
	}
}

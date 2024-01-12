/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:22:16 by mdor              #+#    #+#             */
/*   Updated: 2024/01/12 15:22:18 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_heredoc(t_simple_cmd *cmd, t_minishell *data)
{
	char	*input;
	int		fd[2];

	if (pipe(fd) == -1)
	{
		perror ("pipe");
		data->error_trigger = 1;
		exit(EXIT_FAILURE);
	}
	input = ft_strdup(cmd->heredoc_string, data);
	ft_putstr_fd(input, fd[STDOUT_FILENO]);
	close (fd[STDOUT_FILENO]);
	return (fd[STDIN_FILENO]);
}

int	open_all(t_simple_cmd *cmd)
{
	int	input_fd;

	input_fd = 0;
	while (cmd->input)
	{
		input_fd = open(cmd->input->name, O_RDONLY);
		if (input_fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->input->name, 2);
			ft_putstr_fd(": ", 2);
			perror("");
			break ;
		}
		if (cmd->input->next == NULL)
			break ;
		else
			cmd->input = cmd->input->next;
	}
	return (input_fd);
}

int	create_all_open_last(t_simple_cmd *cmd)
{
	int	output_fd;

	output_fd = 1;
	while (cmd->output)
	{
		if (cmd->append_mode == 0)
			output_fd = open(cmd->output->name, 
					O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else if (cmd->append_mode == 1)
			output_fd = open(cmd->output->name,
					O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (output_fd == -1)
		{
			ft_putstr_fd("minishell :", 2);
			ft_putstr_fd(cmd->output->name, 2);
			ft_putstr_fd(":", 2);
			perror("");
		}
		if (cmd->output->next == NULL)
			break ;
		else
			cmd->output = cmd->output->next;
	}
	return (output_fd);
}

/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::		  */
/*   exec.c											 :+:	  :+:	:+:	      */
/*													+:+ +:+		 +:+		  */
/*   By: mdor <mdor@student.s19.be>				 +#+  +:+	   +#+			  */
/*												+#+#+#+#+#+   +#+			  */
/*   Created: 2023/12/12 09:48:00 by mdor			  #+#	#+#				  */
/*   Updated: 2024/01/11 10:30:55 by mdor			 ###   ########.fr		  */
/*																			  */
/* ************************************************************************** */

//protected sauf les close pipes zzz

#include "minishell.h"

int	execute_builtins(t_simple_cmd *cmd, t_minishell *data)
{
	if (ft_strcmp(cmd->args[0], "echo", data) == 0)
	{
		ft_echo(cmd->args, 1);
		return (1);
	}
	else if (ft_strcmp(cmd->args[0], "pwd", data) == 0)
	{
		ft_pwd(data);
		return (1);
	}
	else if (ft_strcmp(cmd->args[0], "env", data) == 0)
	{
		ft_env(data);
		return (1);
	}
	else
		return (0);
}

void	execute_command(t_simple_cmd *cmd, t_minishell *data)
{
	if (execute_builtins(cmd, data) == 1
		|| ft_strcmp(cmd->args[0], "cd", data) == 0)
	{
		exit(EXIT_SUCCESS);
	}
	else if (execve(cmd->path_to_cmd, cmd->args, data->env) == -1)
	{
		if (ft_strcmp(cmd->args[0], "unset", data) == 0
			|| ft_strcmp(cmd->args[0], "export", data) == 0
			|| ft_strcmp(cmd->args[0], "exit", data) == 0)
			exit(0);
		fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
}

void	execute_simple_cmd(t_simple_cmd *cmd, t_minishell *data, int *pp_fd)
{
	int		pipe_fd[2];
	pid_t	child_pid;

	if (cmd == NULL)
		return ;
	if (cmd->args[0] == NULL)
		return ;
	g_status = 1;
	if (pipe(pipe_fd) == -1)
		return (print_error(1, data));
	handle_builtin(cmd, data, pp_fd);
	child_pid = fork();
	if (child_pid == -1)
		return (print_error(2, data));
	if (child_pid == 0)
		child(cmd, data, pipe_fd, pp_fd);
	else
		parent(cmd, data, pipe_fd, child_pid);
	close(pipe_fd[0]);
	g_status = 0;
}

void	parent(t_simple_cmd *cmd, t_minishell *data, int *p_fd, pid_t child_pid)
{
	int	status;

	close(p_fd[1]);
	if (cmd->next != NULL)
	{
		execute_simple_cmd(cmd->next, data, p_fd);
		waitpid(child_pid, NULL, 0);
	}
	else
	{
		waitpid(child_pid, &status, 0);
		if (WIFSIGNALED(status))
			data->error_trigger = 128 + WTERMSIG(status);
		else if (WIFEXITED(status))
		{
			if (ft_strcmp(data->node->args[0], "cd", data) != 0)
			{
				while (data->node->next != NULL)
					data->node = data->node->next;
				if (ft_strcmp(data->node->args[0], "exit", data) == 0)
					return ;
				data->error_trigger = WEXITSTATUS(status);
			}
		}
	}
}

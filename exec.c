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
	if (strcmp(cmd->args[0], "echo") == 0)
	{
		ft_echo(cmd->args, 1);
		return (1);
	}
	else if (strcmp(cmd->args[0], "pwd") == 0)
	{
		printf("%s\n", ft_pwd());
		return (1);
	}
	else if (strcmp(cmd->args[0], "env") == 0)
	{
		ft_env(data);
		return (1);
	}
	else
		return (0);
}

void	execute_command(t_simple_cmd *cmd, t_minishell *data)
{
	if (execute_builtins(cmd, data) == 1)
	{
		exit(EXIT_SUCCESS);
	}
	else if (execve(cmd->path_to_cmd, cmd->args, data->env) == -1)
	{
		if (ft_strcmp(cmd->args[0], "unset", data) == 0
			|| ft_strcmp(cmd->args[0], "export", data) == 0
			|| ft_strcmp(cmd->args[0], "cd", data) == 0)
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
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		data->error_trigger = 1;
		return ;
	}
	handle_builtin(cmd, data, pp_fd);
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		data->error_trigger = 1;
		return ;
	}
	if (child_pid == 0)
		child(cmd, data, pipe_fd, pp_fd);
	else
		parent(cmd, data, pipe_fd, child_pid);
	close(pipe_fd[0]);
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
		if (WIFEXITED(status))
			data->error_trigger = WEXITSTATUS(status);
	}
}

// void	execute_simple_cmd(t_simple_cmd *cmd, t_minishell *data, int *pp_fd)
// {
// 	int		pipe_fd[2];
// 	int		status;
// 	pid_t	child_pid;

// 	if (cmd == NULL) 
// 		return ;
// 	if (pipe(pipe_fd) == -1) 
// 	{
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (cmd->args != NULL)
// 	{
// 		if (strcmp(cmd->args[0], "export") == 0 && pp_fd == 0)
// 			ft_export(data, cmd->args);
// 		else if (strcmp(cmd->args[0], "unset") == 0 && pp_fd == 0)
// 			ft_unset(data, cmd->args);
// 		else if (strcmp(cmd->args[0], "cd") == 0 && pp_fd == 0)
// 			ft_cd(data, cmd->args[1]);
// 	}
// 	child_pid = fork();
// 	if (child_pid == -1) 
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (child_pid == 0)
// 	{
// 		redirect_input(cmd, pp_fd, data);
// 		redirect_output(cmd, pipe_fd);
// 		execute_command(cmd, data);
// 	}
// 	else
// 	{
// 		close(pipe_fd[1]);
// 		if (cmd->next != NULL)
// 		{
// 			execute_simple_cmd(cmd->next, data, pipe_fd);
// 			waitpid(child_pid, NULL, 0);
// 		}
// 		else
// 		{
// 			waitpid(child_pid, &status, 0);
// 			if (WIFEXITED(status))
// 				data->error_trigger = WEXITSTATUS(status);
// 		}
// 	}
// 	close(pipe_fd[0]);
// }

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

#include "minishell.h"

void	redirect_input(t_simple_cmd *cmd, int *p_fd, t_minishell *data)
{
	int	input_fd;

	input_fd = 0;
	if (cmd->heredoc == 1 && cmd->heredoc_string)
	{
		input_fd = open_all(cmd);
		if (input_fd == -1)
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
	else if (strcmp(cmd->args[0], "exit") == 0)
	{
		ft_exit();
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
		if (ft_strcmp(cmd->args[0], "unset") == 0
			|| ft_strcmp(cmd->args[0], "export") == 0
			|| ft_strcmp(cmd->args[0], "cd") == 0)
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
		exit(EXIT_FAILURE);
	}
	handle_builtin(cmd, data, pp_fd);
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
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

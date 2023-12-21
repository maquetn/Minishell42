/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:48:00 by mdor              #+#    #+#             */
/*   Updated: 2023/12/21 12:36:32 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void close_pipe(int pipe_fd[2]) 
{
    close(pipe_fd[0]);
    close(pipe_fd[1]);
}

void redirect_input(t_simple_cmd *cmd, int *p_fd) 
{
    if (cmd->input != NULL) 
    {
        int input_fd = open(cmd->input, O_RDONLY);
        if (input_fd == -1) 
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    else if (cmd->prev != NULL)
    {
        dup2(p_fd[0], 0);
        close(p_fd[1]);
    }
}

void redirect_output(t_simple_cmd *cmd, int *p_fd) 
{
    if (cmd->output != NULL) 
    {
        int output_fd = open(cmd->output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (output_fd == -1) 
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
    else if (cmd->next != NULL)
    {
        dup2(p_fd[1], 1);
        close(p_fd[0]);
    }
}

void redirect_previous_output(t_simple_cmd *cmd, int pipe_fd[2]) 
{
    if (cmd->prev != NULL) 
    {
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
}

void execute_command(t_simple_cmd *cmd) 
{
    if (strcmp(cmd->args[0], "echo") == 0)
        ft_echo(cmd->args, 1);
    else if (strcmp(cmd->args[0], "cd") == 0)
        ft_cd(cmd->args[1]);
    else if (strcmp(cmd->args[0], "pwd") == 0)
        printf("%s\n", ft_pwd());
    else if (strcmp(cmd->args[0], "export") == 0)
        printf("export function\n");
    else if (strcmp(cmd->args[0], "unset") == 0)
        printf("unset function\n");
    else if (strcmp(cmd->args[0], "env") == 0)
        printf("env function\n");
    else if (strcmp(cmd->args[0], "exit") == 0)
        ft_exit();
    else if(execve(cmd->path_to_cmd, cmd->args, NULL) == -1)
    {
        printf("%s\n", strerror(errno));
        //etre sur de bien liberer les cmd avant de d'exit
        exit(EXIT_FAILURE);
    }
}

void execute_simple_cmd(t_simple_cmd *cmd) 
{
    int pipe_fd[2];
    pid_t child_pid;
 
    if (cmd == NULL) 
        return;
    if (pipe(pipe_fd) == -1) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    child_pid = fork(); 
    //child_pid = 0;
    if (child_pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (child_pid == 0) 
    {
        //close_pipe(pipe_fd);
        redirect_input(cmd, pipe_fd);
        redirect_output(cmd, pipe_fd);
        //redirect_previous_output(cmd, pipe_fd);
        execute_command(cmd);
    }
    else 
    {
        close(pipe_fd[1]);
        waitpid(child_pid, NULL, 0);
        if (cmd->next != NULL) 
        {
            execute_simple_cmd(cmd->next);
        }
    }
}

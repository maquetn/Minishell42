/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:48:00 by mdor              #+#    #+#             */
/*   Updated: 2023/12/16 13:13:34 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void close_pipe(int pipe_fd[2]) 
{
    close(pipe_fd[0]);
    close(pipe_fd[1]);
}

void redirect_input(t_simple_cmd *cmd) 
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
}

void redirect_output(t_simple_cmd *cmd) 
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
}

void redirect_previous_output(t_simple_cmd *cmd, int pipe_fd[2]) 
{
    if (cmd->prev != NULL) 
    {
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
}

void exec_builtin(t_simple_cmd *cmd)
{
    if (cmd == NULL) {
        return;
    }

    if (strcmp(cmd->args[0], "echo") == 0) {
        ft_echo(cmd->args, 1);
    } else if (strcmp(cmd->args[0], "cd") == 0) {
        ft_cd(cmd->args[1]);
    } else if (strcmp(cmd->args[0], "pwd") == 0) {
        printf("%s\n", ft_pwd());
    } else if (strcmp(cmd->args[0], "ls") == 0) {
        ft_ls(ft_pwd());
    } else if (strcmp(cmd->args[0], "export") == 0) {
        printf("export function\n"); // replace with ft_export
    } else if (strcmp(cmd->args[0], "unset") == 0) {
        printf("unset function\n"); // replace with ft_unset
    } else if (strcmp(cmd->args[0], "env") == 0) {
        printf("env function\n"); // replace with ft_env
    } else if (strcmp(cmd->args[0], "exit") == 0) {
        ft_exit();
    }
}

void execute_simple_cmd(t_simple_cmd *cmd) 
{
    int pipe_fd[2];
    pid_t child_pid;

    if (cmd == NULL) 
    {
        return;
    }

    if (pipe(pipe_fd) == -1) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    //child_pid = fork(); COUPABLE
    child_pid = 0;

    if (child_pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) 
    {
        close_pipe(pipe_fd);
        redirect_input(cmd);
        redirect_output(cmd);
        redirect_previous_output(cmd, pipe_fd);
        exec_builtin(cmd);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:48:00 by mdor              #+#    #+#             */
/*   Updated: 2024/01/05 13:15:02 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int get_dollar(char *str, int i)
// {
//     while (str[i] != '$' && str[i] != '\0')
//         i++;
//     return (i);
// }

// char    *heredoc_dollar(char *str, t_minishell *data)
// {
//     char    *translated = NULL;
//     char    *placeholder = NULL;
//     char    *placeholder2 = NULL;
//     int code;
//     int i;

//     i = 0;
//     translated = ft_strdup("", data);
//     code = 0;
//     while (str[i] != '\0')
//     {
//         if (str[i] == '$' && str[i + 1] == '$')
//         {
//             code = (int)getpid();
//             placeholder = ft_itoa(code, data);
//             translated = ft_strjoin(translated, placeholder, data);
//             i += 2;
//         }
//         else if (str[i] == '$' && str[i + 1] == '?')
//         {
//             code = data->exit_code;
// 		    placeholder = ft_itoa(code, data);
// 		    translated = ft_strjoin(translated, placeholder, data);
//             i += 2;
//         }
//         else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
//         {
//             placeholder2 = ft_strndup(str, i + 1, get_cancer(str, i + 1) - 1, data);
//             placeholder = get_env(placeholder2, data->env, data);
//             if (placeholder == NULL)
//             	placeholder = ft_strdup("", data);
//             translated = ft_strjoin(translated, placeholder, data);
//             i = get_cancer(str, i + 1);
//         }
//         else if (str[i] == '$')
//         {
//             translated = ft_strjoin(translated, "$", data);
//             i++;
//         }
//         else
//         {
//             translated = ft_strjoin(translated, ft_strndup(str, i, get_dollar(str, i + 1) - 1, data), data);
//             i = get_dollar(str, i);
//         }
//     }
//     translated = ft_strjoin(translated, "\n", data);
//     printf("trans :%s\n", translated);
//     return (translated);
// }

// char    *heredoc_expander(char *str, char **file, t_minishell *data)
// {
//     char    *translated;

//     translated = heredoc_dollar(str, data);
//     *file = ft_strjoin(*file, translated, data);
//     return (*file);
// }

// char    *manage_heredoc(char *delim, t_minishell *data)
// {
//     char    *file;
//     char    *input;

//     file = ft_strdup("", data);
//     while (1)
//     {
//         input = readline("> ");
//         if (ft_strcmp(delim, input) == 0)
//             break;
//         else
//             file = heredoc_expander(input, &file, data);
//         free(input);
//     }
//     free(input);
//     return (file);
// }

void close_pipe(int pipe_fd[2]) 
{
    close(pipe_fd[0]);
    close(pipe_fd[1]);
}

int redirect_heredoc(t_simple_cmd *cmd, t_minishell *data)
{
    char    *input;
    int fd[2];


    if (pipe(fd) == -1)
        EXIT_FAILURE;
    input = ft_strdup(cmd->input, data);
    ft_putstr_fd(input, fd[STDOUT_FILENO]);
    close (fd[STDOUT_FILENO]);
    return (fd[STDIN_FILENO]);
}

void redirect_input(t_simple_cmd *cmd, int *p_fd, t_minishell *data) 
{
    int input_fd;

    input_fd = 0;
    if (cmd->input != NULL && cmd->heredoc == 1)
    {
        input_fd = redirect_heredoc(cmd, data);
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    else if (cmd->input != NULL) 
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
    else if (cmd->prev != NULL && p_fd)
    {
        //printf("input\n");
        dup2(p_fd[0], 0);
        close(p_fd[1]);
    }
}

int create_all_open_last(t_simple_cmd *cmd)
{
    int output_fd;

    output_fd = 1;
    while (cmd->output)
    {
        if (cmd->append_mode == 0)
            output_fd = open(cmd->output->name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        else if (cmd->append_mode == 1)
            output_fd = open(cmd->output->name, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (cmd->output->next == NULL)
            break;
        else
            cmd->output = cmd->output->next;
    }
    return (output_fd);
}

void redirect_output(t_simple_cmd *cmd, int *p_fd) 
{
	int output_fd;

    output_fd = 1;
    if (cmd->output != NULL) 
    {
        output_fd = create_all_open_last(cmd);
        if (output_fd == -1) 
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
    else if (cmd->next != NULL && p_fd)
    {
        //printf("output\n");
        dup2(p_fd[1], 1);
        close(p_fd[0]);
    }
}

// void redirect_previous_output(t_simple_cmd *cmd, int pipe_fd[2]) 
// {
//     if (cmd->prev != NULL)
//     {
//         printf("usefull\n");
//         dup2(pipe_fd[1], STDOUT_FILENO);
//         close(pipe_fd[1]);
//     }
// }

int execute_builtins(t_simple_cmd *cmd, t_minishell *data)
{
    //dans lideal il faudrait que les builtins renvoient 1 si SUCCESS
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
    // else if (strcmp(cmd->args[0], "export") == 0)
    // {
    //     printf("export function\n");
    //     return (1);
    // }
    // else if (strcmp(cmd->args[0], "unset") == 0)
    // {
    //     printf("unset function\n");
    //     return (1);
    // }
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

void execute_command(t_simple_cmd *cmd, t_minishell *data) 
{
    if (execute_builtins(cmd, data) == 1)
    {
        exit(EXIT_SUCCESS);
    }
    else if(execve(cmd->path_to_cmd, cmd->args, NULL) == -1)
    {
        fprintf(stderr, "%s : cmd not found\n", cmd->args[0]);
        //etre sur de bien liberer les cmd avant de d'exit
        exit(127);
    }
}

void execute_simple_cmd(t_simple_cmd *cmd, t_minishell *data, int *prev_pipe_fd) 
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
    if (cmd->args != NULL)
    {
        if (strcmp(cmd->args[0], "export") == 0 && prev_pipe_fd == 0)
            ft_export(data, cmd->args);
        else if (strcmp(cmd->args[0], "unset") == 0 && prev_pipe_fd == 0)
            ft_unset(data, cmd->args);
    }
    child_pid = fork();
    if (child_pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (child_pid == 0)
    {
        //close_pipe(pipe_fd);
        redirect_input(cmd, prev_pipe_fd, data);
        redirect_output(cmd, pipe_fd);
        // if (cmd->prev)
        //     redirect_previous_output(cmd->prev, pipe_fd);
        // je dois wait pid avant d'exec ?!
        execute_command(cmd, data);
    }
    else
    {
        close(pipe_fd[1]);
        if (cmd->next != NULL)
        {
            execute_simple_cmd(cmd->next, data, pipe_fd);
        }
        waitpid(child_pid, NULL, 0);
    }
}
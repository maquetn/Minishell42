/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 12:46:45 by nmaquet           #+#    #+#             */
/*   Updated: 2023/12/06 13:29:45 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_builtin(char *token_content, t_token *tokens, int arg)
{
    (void) arg;

    if (strcmp(token_content, "echo") == 0) {
        printf("echo function\n"); // replace with ft_echo
    } else if (strcmp(token_content, "cd") == 0) {
        printf("cd function\n"); // replace with ft_cd
    } else if (strcmp(token_content, "pwd") == 0) {
        printf("pwd function\n"); // replace with ft_pwd
    } else if (strcmp(token_content, "export") == 0) {
        printf("export function\n"); // replace with ft_export
    } else if (strcmp(token_content, "unset") == 0) {
        printf("unset function\n"); // replace with ft_unset
    } else if (strcmp(token_content, "env") == 0) {
        printf("env function\n"); // replace with ft_env
    } else if (strcmp(token_content, "exit") == 0) {
        printf("exit function\n"); // replace with ft_exit
        // Optionally add code to handle the exit command
    } else {
        printf("%s: command not found\n", tokens[0].content);
        return 1;  // Indicate that the command was not found
    }

    return 0;  // Indicate that the command was found and executed
}

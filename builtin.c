/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 12:46:45 by nmaquet           #+#    #+#             */
/*   Updated: 2023/12/04 13:23:53 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_builtin(char *token_content, t_token *tokens, int arg)
{
	(void) arg;
	
	if (strcmp(token_content, "echo") == 0)
		printf("echo function\n"); // replace with ft_echo

	else if (strcmp(token_content, "cd") == 0)
		printf("cd function\n"); // replace with ft_echo

	else if (strcmp(token_content, "pwd") == 0)
		printf("pwd function\n"); // replace with ft_echo

	else if (strcmp(token_content, "export") == 0)
		printf("export function\n"); // replace with ft_echo

	else if (strcmp(token_content, "unset") == 0)
		printf("unset function\n"); // replace with ft_echo

	else if (strcmp(token_content, "env") == 0)
		printf("env function\n"); // replace with ft_echo

	else if (strcmp(token_content, "exit") == 0)
		printf("exit function\n"); // replace with ft_echo

	else
		printf("%s: command not found\n", tokens[0].content);

	return (0);
}

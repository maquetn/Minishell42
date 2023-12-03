/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 12:46:45 by nmaquet           #+#    #+#             */
/*   Updated: 2023/12/03 13:05:40 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_builtin(char *token_content)
{
	if (strcmp(token_content, "echo") == 0)
		printf("echo function\n"); // replace with ft_echo
	return (0);
}

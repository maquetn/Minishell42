/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 10:34:53 by mdor              #+#    #+#             */
/*   Updated: 2024/01/09 10:34:55 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int print_syntax_error(t_token *token)
{
    ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
    if (token->next != NULL)
        ft_putstr_fd(token->next->content, 2);
    else
        ft_putstr_fd("newline", 2);
    ft_putstr_fd("'\n", 2);
    return (1);
}
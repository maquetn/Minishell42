/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <mdor@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 10:34:53 by mdor              #+#    #+#             */
/*   Updated: 2024/01/10 14:43:48 by mdor             ###   ########.fr       */
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
    return (258);//determiner le bon code d'erreur pour une erreur de syntaxe
}
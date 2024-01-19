/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:28:41 by mdor              #+#    #+#             */
/*   Updated: 2024/01/19 14:02:30 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quoted(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

void	rewind_tokens(t_minishell *data)
{
	while (data->first_token->prev)
		data->first_token = data->first_token->prev;
}

int	_next(char *str, int i)
{
	while (str[i] != '"' && str[i] != '\'' 
		&& str[i] != '$' && str[i] != '\0' && str[i] != ' ')
		i++;
	return (i);
}

int	quotes_dollar_gtr(char *str, int i)
{
	while (str[i] != '"' && str[i] != '\'' && str[i] != '$' && str[i] != '\0' )
		i++;
	return (i);
}

int	remove_single(char *str, int i, char **expanded, t_minishell *data)
{
	int	start;

	i++;
	start = i;
	while (str[i])
	{
		if (str[i] == '\'')
			break ;
		i++;
	}
	*expanded = ft_strjoin(*expanded, 
			ft_strndup(str, start, i - 1, data), data);
	return (i);
}

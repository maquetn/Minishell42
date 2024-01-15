/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 20:46:31 by mdor              #+#    #+#             */
/*   Updated: 2024/01/12 20:46:33 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	need_refine(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '"' || str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

void	expand_heredoc(t_token *token, t_minishell *data)
{
	if (quoted(token->content))
		token->quoted_heredoc = 1;
	token->content = heredoc_delim(token->content, data);
}

int	remove_double(char *str, int i, char **exp, t_minishell *data)
{
	int	start;

	i++;
	start = i;
	while (str[i])
	{
		if (str[i] == '"')
			break ;
		else if (str[i] == '$')
		{
			*exp = ft_strjoin(*exp, ft_strndup(str, start, i - 1, data), data);
			data->quoted = 1;
			i = dollar(str, i, exp, data);
			start = i;
			continue ;
		}
		i++;
	}
	data->quoted = 0;
	*exp = ft_strjoin(*exp, ft_strndup(str, start, i - 1, data), data);
	return (i);
}

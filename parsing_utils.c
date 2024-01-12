/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 19:11:25 by mdor              #+#    #+#             */
/*   Updated: 2024/01/12 19:11:27 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			i = dollar(str, i, exp, data, 1);
			start = i;
			continue ;
		}
		i++;
	}
	*exp = ft_strjoin(*exp, ft_strndup(str, start, i - 1, data), data);
	return (i);
}

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

char *heredoc_delim(char *str, t_minishell *data)
{
    int len = 0, i = 0;
    bool in_quote = false;
	char	*translated = NULL;

    while (str[i] != '\0')
	{
	  if (str[i] == '$' && (str[i + 1] == '\'' || str[i + 1] == '\"') && !in_quote)
		{
		i += 2;
		continue;
	  }
	  if ((str[i] == '\'' || str[i] == '\"') && !in_quote)
		{
		in_quote = true;
		i++;
		continue;
	  }
		else if ((str[i] == '\'' || str[i] == '\"') && in_quote)
		{
		in_quote = false;
		i++;
		continue;
	  }
	  len++;
	  i++;
    }
    translated = gc_malloc(len + 1, data); 
    if (translated == NULL)
		return NULL;

    int j = 0;
    i = 0;
    in_quote = false;
    while (str[i] != '\0')
	{
	  if (str[i] == '$' && ((str[i + 1] == '\'' || str[i + 1] == '\"') && str[i + 2] != '\0') && !in_quote)
		{
		i += 2;
		continue;
	  }
	  if ((str[i] == '\'' || str[i] == '\"') && !in_quote)
		{
		in_quote = true;
		i++;
		continue;
	  }
		else if ((str[i] == '\'' || str[i] == '\"') && in_quote)
		{
		in_quote = false;
		i++;
		continue;
	  }
	  translated[j++] = str[i];
	  i++;
    }
    translated[j] = '\0';
    return (translated);
}

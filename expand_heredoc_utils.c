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

char	*build_translated_string(char **str, int len, t_minishell *data)
{
	char	*translated;
	int		i;
	int		j;
	bool	in_quote;

	init_vars(translated, &i, &j, &in_quote);
	translated = gc_malloc(len + 1, data); 
	if (translated == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		if (is_escape_sequence(str, i) && !in_quote)
		{
			i += 2;
			continue ;
		}
		if (is_quote_char(str[i]))
		{
			in_quote = !in_quote;
			i++;
			continue ;
		}
		translated[j++] = str[i];
		i++;
	}
	translated[j] = '\0';
	return (translated);
}

bool is_quote_char(char c)
{
	return (c == '\'' || c == '\"');
}

bool is_escape_sequence(const char *str, int index)
{
	return (str[index] == '$' && is_quote_char(str[index + 1]));
}

char *heredoc_delim(char *str, t_minishell *data)
{
	int len; 

	len = calculate_translated_length(str);
	return (build_translated_string(str, len, data));
}
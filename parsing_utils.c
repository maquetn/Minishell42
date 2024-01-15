/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::		  */
/*   quotes_utils.c									 :+:	  :+:	:+:	      */
/*													+:+ +:+		 +:+		  */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+			  */
/*   Created: 2024/01/12 19:11:25 by mdor			  #+#	#+#				  */
/*   Updated: 2024/01/12 19:11:27 by mdor			 ###   ########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

int	calculate_translated_length(const char *str)
{
	int		len;
	int		i;
	bool	in_quote;

	in_quote = false;
	i = 0;
	len = 0;
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
		len++;
		i++;
	}
	return (len);
}

char	*build_translated_string(char *str, int len, t_minishell *data)
{
	char	*translated;
	int		i;
	int		j;
	bool	in_quote;

	j = 0; 
	i = -1;
	in_quote = false;
	translated = gc_malloc(len + 1, data); 
	if (translated == NULL)
		return (NULL);
	while (str[++i] != '\0')
	{
		if (is_escape_sequence(str, i) && !in_quote)
			i += 1;
		else if (is_quote_char(str[i]))
			in_quote = !in_quote;
		else
			translated[j++] = str[i];
	}
	translated[j] = '\0';
	return (translated);
}

bool	is_quote_char(char c)
{
	return (c == '\'' || c == '\"');
}

bool	is_escape_sequence(const char *str, int index)
{
	return (str[index] == '$' && is_quote_char(str[index + 1]));
}

char	*heredoc_delim(char *str, t_minishell *data)
{
	int	len; 

	len = calculate_translated_length(str);
	return (build_translated_string(str, len, data));
}

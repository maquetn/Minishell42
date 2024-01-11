/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::        */
/*   heredoc.c										  :+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2024/01/08 13:25:13 by mdor			  #+#	#+#			      */
/*   Updated: 2024/01/08 13:25:23 by mdor			 ###   ########.fr	      */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

char	*process_single_dollar(char *translated, int *i, t_minishell *data)
{
	*i += 1;
	return (ft_strjoin(translated, "$", data));
}

char	*process_other_chars(char *str, char *trans, int *i, t_minishell *data)
{
	trans = ft_strjoin(trans, 
			ft_strndup(str, *i, get_dollar(str, *i + 1) - 1, data), data);
	*i = get_dollar(str, *i);
	return (trans);
}

char	*heredoc_expander(char *str, char **file, t_minishell *data)
{
	char	*translated;

	translated = heredoc_dollar(str, data, 0);
	*file = ft_strjoin(*file, translated, data);
	return (*file);
}

char	*manage_heredoc(char *delim, t_minishell *data)
{
	char	*file;
	char	*input;

	printf("delim : %s\n", delim);
	file = ft_strdup("", data);
	while (1)
	{
		input = readline("> ");
		if (ft_strcmp(delim, heredoc_dollar(input, data, 1)) == 0)
			break ;
		else
			file = heredoc_expander(input, &file, data);
		free(input);
	}
	free(input);
	return (file);
}

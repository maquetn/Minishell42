/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::        */
/*   token.c											:+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2024/01/14 18:20:07 by mdor			  #+#	#+#			      */
/*   Updated: 2024/01/14 18:20:08 by mdor			 ###   ########.fr	      */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"	

int	inp_out_pipe(char *str, int i, t_token **head, t_minishell *data)
{
	if (str[i] == '>')
		i = manage_output_append(str, i, head, data) + 1;
	else if (str[i] == '<')
		i = manage_input_heredoc(str, i, head, data) + 1;
	else if (str[i] == '|')
		i = manage_pipe(i, head, data) + 1;
	return (i);
}

int	quotes(char *str, int i, char **content, t_minishell *data)
{
	if (str[i] == '\'')
		i = manage_single_quotes (str, i, content, data);
	else if (str[i] == '"')
		i = manage_double_quotes(str, i, content, data);
	return (i);
}

void	add_after_space(char **c, t_token **head, t_minishell *data)
{
	add_token(head, STR, *c, data);
	*c = ft_strdup("", data);
}

int	add_normal_str(char	*str, int i, char **content, t_minishell *data)
{
	*content = ft_strjoin(*content,
			ft_strndup(str, i, get_spc_chr(str, i), data), data);
	return (i = get_next_token(str, i));
}

void	add_token_str(t_token **head, char **content, t_minishell *data)
{
	add_token(head, STR, *content, data);
	*content = ft_strdup("", data);
}

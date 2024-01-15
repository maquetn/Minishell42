/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_adder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 03:48:54 by mdor              #+#    #+#             */
/*   Updated: 2024/01/15 03:48:55 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	manage_double_quotes(char *str, int i, char **content, t_minishell *data)
{
	int	start;

	start = i + 1;
	while (str[i])
	{
		i++;
		if (str[i] == '\"' )
		{
			*content = ft_strjoin(*content,
					ft_strndup(str, start - 1, i, data), data);
			return (i);
		}
	}
	ft_putstr_fd("Hey hey dont try unclosed quotes here\n", 2);
	data->error_trigger = 1;
	return (i);
}

int	manage_single_quotes(char *str, int i, char **content, t_minishell *data)
{
	int	start;

	start = i + 1;
	while (str[i])
	{
		i++;
		if (str[i] == '\'' )
		{
			*content = ft_strjoin(*content,
					ft_strndup(str, start - 1, i, data), data);
			return (i);
		}
	}
	ft_putstr_fd("Hey hey dont try unclosed quotes here\n", 2);
	data->error_trigger = 1;
	return (i);
}

int	manage_output_append(char *str, int i, t_token **head, t_minishell *data)
{
	if (str[i + 1] == '>')
	{
		i++;
		add_token(head, APPEND, ">>", data);
	}
	else
		add_token(head, OUTPUT, ">", data);
	return (i);
}

int	manage_input_heredoc(char *str, int i, t_token **head, t_minishell *data)
{
	if (str[i + 1] == '<')
	{
		i++;
		add_token(head, HEREDOC, "<<", data);
	}
	else
		add_token(head, INPUT, "<", data);
	return (i);
}

int	manage_pipe(int i, t_token **head, t_minishell *data)
{
	add_token(head, PIPE, "|", data);
	return (i);
}

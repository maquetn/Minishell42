/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 06:08:03 by mdor              #+#    #+#             */
/*   Updated: 2024/01/15 06:08:05 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_space(char *str, int *i, t_t_args *args)
{
	if (*i > 0 && (str[*i - 1] == '"' || str[*i - 1] == '\''))
	{
		add_after_space(args->content, args->head, args->data);
	}
	*i += 1;
}

void	handle_special_chars(char *str, int *i, t_t_args *args)
{
	if (str[*i] == '>' || str[*i] == '<' || str[*i] == '|')
	{
		*i = inp_out_pipe(str, *i, args->head, args->data);
	}
}

void	handle_quotes(char *str, int *i, t_t_args *args)
{
	if (str[*i] == '\'' || str[*i] == '"')
	{
		*i = quotes(str, *i, args->content, args->data);
	}
}

void	process_string(char *str, int *i, t_t_args *args)
{
	while (str[*i] && args->data->error_trigger == 0 && *i < ft_strlen(str))
	{
		if (str[*i] == ' ')
		{
			handle_space(str, i, args);
			continue ;
		}
		handle_special_chars(str, i, args);
		handle_quotes(str, i, args);
		if (str[*i] == '\'' || str[*i] == '"')
		{
			if (str[++(*i)] == '\0')
				add_token(args->head, STR, *(args->content), args->data);
		}
		else
		{
			*i = add_normal_str(str, *i, args->content, args->data);
			if (str[*i] != '\'' && str[*i] != '"')
				add_token_str(args->head, args->content, args->data);
			if (str[*i + 1] == '\0' || args->data->error_trigger != 0)
				break ;
		}
	}
}

void	token(char *str, t_minishell *data)
{
	int			i;
	char		*content;
	t_token		*head;
	t_t_args	args;

	i = 0;
	head = NULL;
	content = ft_strdup("", data);
	args.content = &content;
	args.head = &head;
	args.data = data;
	process_string(str, &i, &args);
	data->first_token = head;
}
	//print_tokens(head);

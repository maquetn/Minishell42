/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 03:39:48 by mdor              #+#    #+#             */
/*   Updated: 2024/01/15 03:39:49 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *str, int start, int end, t_minishell *data)
{
	int		len;
	char	*dup;
	int		i;

	len = end - start + 1;
	i = 0;
	dup = gc_malloc(sizeof(char) * (len + 1), data);
	if (dup == NULL)
		return (NULL);
	while (i < len)
	{
		dup[i] = str[start + i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

int	get_next_token(char *str, int start)
{
	while (str[start])
	{
		if (str[start] == '>' || str[start] == '<' || str[start] == '|'
			|| str[start] == ' ' || str[start] == '"'
			|| str[start] == '\'' || str[start] == '\0')
		{
			return (start);
		}
		start++;
	}
	return (start - 1);
}

int	get_spc_chr(char *str, int start)
{
	while (str[start])
	{
		if (str[start] == '>' || str[start] == '<'
			|| str[start] == '|' || str[start] == ' '
			|| str[start] == '"' || str[start] == '\'')
		{
			return (start - 1);
		}
		start++;
	}
	return (start);
}

void	add_token(t_token **head, t_token_type type,
	char *content, t_minishell *data)
{
	t_token	*new_token;
	t_token	*current;

	new_token = NULL;
	current = NULL;
	if (!content || content[0] == '\0')
		return ;
	new_token = gc_malloc(sizeof(t_token), data);
	if (new_token == NULL)
		return ;
	new_token->type = type;
	new_token->content = ft_strdup(content, data);
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->quoted_heredoc = 0;
	if (*head == NULL) 
		*head = new_token;
	else 
	{
		current = *head;
		while (current->next != NULL) 
			current = current->next;
		current->next = new_token;
		new_token->prev = current;
	}
}

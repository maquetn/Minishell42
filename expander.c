/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::		  */
/*   expander.c										 :+:	  :+:	:+:		  */
/*													+:+ +:+		 +:+		  */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+			  */
/*   Created: 2024/01/05 17:14:18 by mdor			  #+#	#+#				  */
/*   Updated: 2024/01/05 18:18:45 by mdor			 ###   ########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

int	dollar(char *str, int i, char **expanded, t_minishell *data, int quoted)
{
	char	*translated = NULL;

	while (str[i] && str[i] == '$')
	{
		if (str[i] == '$' && str[i + 1] == '$')
		{
			*expanded = ft_strjoin(*expanded, "$$", data);
			i += 2;
		}
		else if (str[i] == '$' && str[i + 1] == '?')
		{
			*expanded = ft_strjoin(*expanded, ft_itoa(data->code, data), data);
			i += 2;
		}
		else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
		{
			translated = get_env(ft_strndup(str, i + 1,
						_next(str, i + 1) - 1, data), data->env, data);
			if (translated == NULL)
				translated = ft_strdup("", data);
			*expanded = ft_strjoin(*expanded, translated, data);
			i = _next(str, i + 1);
		}
		else if (quoted == 0)
		{
			if (str[i] == '$' && str[i + 1] == '"' && str[i - 1] != '"')
				i = remove_double(str, i + 1, expanded, data);
			else if (str[i] == '$' && str[i + 1] == '\'' && str[i - 1] != '\'')
				i = remove_single(str, i + 1, expanded, data) + 1;
			else if (str[i] == '$')
			{
				*expanded = ft_strjoin(*expanded, "$", data);
				i++;
			}
		}
		else if (str[i] == '$')
		{
			*expanded = ft_strjoin(*expanded, "$", data);
			i++;
		}
	}
	return (i);
}

void	fine_touch(t_token *t, t_minishell *data)
{
	int		i;
	char	*expanded;

	i = 0;
	expanded = ft_strdup("", data);
	while (t->content[i] != '\0' || i < ft_strlen(t->content))
	{
		if (t->content[i] == '\'')
			i = remove_single(t->content, i, &expanded, data);
		else if (t->content[i] == '"')
			i = remove_double(t->content, i, &expanded, data);
		else if (t->content[i] == '$')
		{
			i = dollar(t->content, i, &expanded, data, 0);
			if (i >= 2 && (t->content[i] == '\'' || (t->content[i - 1] == '$'
						&& t->content[i - 2] == '$')))
					continue ;
		}
		else
		{
			expanded = ft_strjoin(expanded, ft_strndup(t->content,
						i, get_cancer(t->content, i) - 1, data), data);
			i = get_cancer(t->content, i);
			continue ;
		}
		i++;
	}
	t->content = ft_strdup(expanded, data);
}

void	expander(t_minishell *data)
{
	int	heredoc;

	heredoc = 0;
	while (data->first_token)
	{
		if (data->first_token->prev != NULL)
		{
			if (data->first_token->type == STR 
				&& data->first_token->prev->type == HEREDOC 
				&& need_refine(data->first_token->content))
			{
				heredoc = 1;
				expand_heredoc(data->first_token, data);
			}
		}
		if (data->first_token->type == STR 
			&& need_refine(data->first_token->content) && heredoc == 0)
			fine_touch(data->first_token, data);
		if (data->first_token->next == NULL)
			break ;
		data->first_token = data->first_token->next;
		heredoc = 0;
	}
	rewind_tokens(data);
}
	//print_tokens(data->first_token);

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

int	dollar(char *str, int i, char **expanded, t_minishell *data)
{
	while (str[i] && str[i] == '$')
	{
		if (str[i] == '$' && (str[i + 1] == '$' || str[i + 1] == '?'))
			i = doldol_dolmark(expanded, str[i + 1], data, i);
		else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			i = expand_env(str, i, expanded, data);
		else if (data->quoted == 0)
			i = check_quotes(str, i, expanded, data);
		else if (str[i] == '$')
		{
			*expanded = ft_strjoin(*expanded, "$", data);
			i++;
		}
	}
	return (i);
}

int	sing_double(t_token *token, int i, char **expanded, t_minishell *data)
{
	if (token->content[i] == '\'')
		return (remove_single(token->content, i, expanded, data));
	else if (token->content[i] == '"')
		return (remove_double(token->content, i, expanded, data));
	return (i);
}

int	continue_condition(int i, t_token *t)
{
	if (i >= 2 && (t->content[i] == '\'' || ((t->content[i - 1] == '$'
					|| t->content[i - 1] == '?')
				&& t->content[i - 2] == '$')))
		return (1);
	else
		return (0);
}

void	fine_touch(t_token *t, t_minishell *data)
{
	int		i;
	char	*expanded;

	i = 0;
	expanded = ft_strdup("", data);
	while ((i - 1) < ft_strlen(t->content) && t->content[i] != '\0')
	{
		if (t->content[i] == '\'' || t->content[i] == '"')
			i = sing_double(t, i, &expanded, data);
		else if (t->content[i] == '$')
		{
			i = dollar(t->content, i, &expanded, data);
			if (continue_condition(i, t) == 1)
				continue ;
		}
		else
		{
			expanded = ft_strjoin(expanded, ft_strndup(t->content,
						i, quotes_dollar_gtr(t->content, i) - 1, data), data);
			i = quotes_dollar_gtr(t->content, i) - 1;
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

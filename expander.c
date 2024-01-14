/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::	      */
/*   expander.c										 :+:	  :+:	:+:	      */
/*													+:+ +:+		 +:+		  */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+			  */
/*   Created: 2024/01/05 17:14:18 by mdor			  #+#	#+#				  */
/*   Updated: 2024/01/05 18:18:45 by mdor			 ###   ########.fr		  */
/*																			  */
/* ************************************************************************** */

//protected

#include "minishell.h"	

int	dollar(char *str, int i, char **expanded, t_minishell *data, int quoted)
{
	char	*translated = NULL;
	int		code;
	char	*placeholder = NULL;

	code = 0;
	while (str[i] && str[i] == '$')
	{
		if (str[i] == '$' && str[i + 1] == '$')
		{
			translated = ft_strdup("$$", data);
			*expanded = ft_strjoin(*expanded, translated, data);
			i += 2;
		}
		else if (str[i] == '$' && str[i + 1] == '?')
		{
			code = data->exit_code;
			translated = ft_itoa(code, data);
			*expanded = ft_strjoin(*expanded, translated, data);
			i += 2;
		}
		else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
		{
			placeholder = ft_strndup(str, i + 1, _next(str, i + 1) - 1, data);
			translated = get_env(placeholder, data->env, data);
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

void	fine_touch(t_token *token, t_minishell *data)
{
	char	*translated = NULL;
	int		i;
	char	*expanded = NULL;

	translated = ft_strdup(token->content, data);
	i = 0;
	expanded = ft_strdup("", data);
	while (translated[i] != '\0')
	{
		if (translated[i] == '\'')
			i = remove_single(translated, i, &expanded, data);
		else if (translated[i] == '"')
			i = remove_double(translated, i, &expanded, data);
		else if (translated[i] == '$')
		{
			i = dollar(translated, i, &expanded, data, 0);
			if (i >= 2)
				if (translated[i] == '\'' || (translated[i - 1] == '$' && translated[i - 2] == '$'))
					continue ;
		}
		else
		{
			expanded = ft_strjoin(expanded, ft_strndup(translated, i, get_cancer(translated, i) - 1, data), data);
			i = get_cancer(translated, i);
			continue ;
		}
		if (i >= ft_strlen(translated))
			break ;
		i++;
	}
	token->content = ft_strdup(expanded, data);
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

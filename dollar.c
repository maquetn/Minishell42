/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 09:36:01 by mdor              #+#    #+#             */
/*   Updated: 2024/01/15 09:36:07 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	doldol_dolmark(char **expanded, char nextchr, t_minishell *data, int i)
{
	if (nextchr == '$')
		*expanded = ft_strjoin(*expanded, "$$", data);
	else if (nextchr == '?')
		*expanded = ft_strjoin(*expanded, ft_itoa(data->code, data), data);
	return (i + 2);
}

int	expand_env(char *str, int i, char **expanded, t_minishell *data)
{
	char	*trans;

	trans = get_env(ft_strndup(str, i + 1,
				_next(str, i + 1) - 1, data), data->env, data);
	if (trans == NULL)
		trans = ft_strdup("", data);
	*expanded = ft_strjoin(*expanded, trans, data);
	i = _next(str, i + 1);
	return (i);
}

int	check_quotes(char *str, int i, char **expanded, t_minishell *data)
{
	if (str[i] == '$' && str[i + 1] == '"' && data->quoted == 0)
		i = remove_double(str, i + 1, expanded, data);
	else if (str[i] == '$' && str[i + 1] == '\'' && str[i - 1] != '\'')
		i = remove_single(str, i + 1, expanded, data) + 1;
	else if (str[i] == '$')
	{
		*expanded = ft_strjoin(*expanded, "$", data);
		i++;
	}
	return (i);
}

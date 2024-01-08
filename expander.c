/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 17:14:18 by mdor              #+#    #+#             */
/*   Updated: 2024/01/08 12:11:58 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_cancer(char *str, int i)
{
    while (str[i] != '"' && str[i] != '\'' && str[i] != '$' && str[i] != '\0')
        i++;
    return (i);
}

int remove_single(char *str, int i, char **expanded, t_minishell *data)
{
    int start;

    i++;
    start = i;
    while (str[i])
    {
        if (str[i] == '\'')
            break;
        i++;
    }
    *expanded = ft_strjoin(*expanded, ft_strndup(str, start, i - 1, data), data);
    return (i);
}

int	dollar(char *str, int i, char **expanded, t_minishell *data, int coming_from_quote)
{
    char *translated = NULL;
    pid_t pid;
    int     code;
    char    *placeholder = NULL;

    code = 0;
    pid = 0;
    while (str[i] && str[i] == '$')
    {
        if (str[i] == '$' && str[i + 1] == '$')
        {
            pid = getpid();
            translated = ft_itoa(pid, data);
            *expanded = ft_strjoin(*expanded, translated, data);
            i += 2;
            printf("str i : %c\n", str[i]);
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
            placeholder = ft_strndup(str, i + 1, get_cancer(str, i + 1) - 1, data);
            translated = get_env(placeholder, data->env, data);
            if (translated == NULL)
            	translated = ft_strdup("", data);
            *expanded = ft_strjoin(*expanded, translated, data);
            i = get_cancer(str, i + 1);
        }
        else if (coming_from_quote == 0)
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

int remove_double(char *str, int i, char **expanded, t_minishell *data)
{
    int start;

    i++;
    start = i;
    while (str[i])
    {
        if (str[i] == '"')
            break;
        else if (str[i] == '$')
        {
            *expanded = ft_strjoin(*expanded, ft_strndup(str, start, i - 1, data), data);
            i = dollar(str, i, expanded, data, 1);
            start = i;
            continue;
        }
        i++;
    }
    *expanded = ft_strjoin(*expanded, ft_strndup(str, start, i - 1, data), data);
    return (i);
}


void	fine_touch(t_token *token, t_minishell *data)
{
	char	*translated = NULL;
	int		i;
    char    *expanded = NULL;

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
            if (translated[i] == '\'' || (translated[i - 1] == '$' && translated[i - 2] == '$'))
                continue;
        }
        else
        {
            expanded = ft_strjoin(expanded, ft_strndup(translated, i, get_cancer(translated, i) - 1, data), data);
            i = get_cancer(translated, i);
            continue;
        }
        if (i >= ft_strlen(translated))
            break;
        i++;
    }
    token->content = ft_strdup(expanded, data);
}

int need_refine(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '\'' || str[i] == '"' ||str[i] == '$')
            return (1);
        i++;
    }
    return (0);
}
void rewind_tokens(t_minishell *data)
{
    while (data->first_token->prev)
        data->first_token = data->first_token->prev;
}

void	expander(t_minishell *data)
{
	while (data->first_token)
	{
		if (data->first_token->type == STR && need_refine(data->first_token->content))
			fine_touch(data->first_token, data);
        if (data->first_token->next == NULL)
            break;
		data->first_token = data->first_token->next;
	}
    rewind_tokens(data);
    //print_tokens(data->first_token);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:25:13 by mdor              #+#    #+#             */
/*   Updated: 2024/01/08 13:25:23 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int get_dollar(char *str, int i)
{
    while (str[i] != '$' && str[i] != '\0')
        i++;
    return (i);
}

char    *heredoc_dollar(char *str, t_minishell *data)
{
    char    *translated = NULL;
    char    *placeholder = NULL;
    char    *placeholder2 = NULL;
    int code;
    int i;

    i = 0;
    translated = ft_strdup("", data);
    code = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '$' && str[i + 1] == '$')
        {
            code = (int)getpid();
            placeholder = ft_itoa(code, data);
            translated = ft_strjoin(translated, placeholder, data);
            i += 2;
        }
        else if (str[i] == '$' && str[i + 1] == '?')
        {
            code = data->exit_code;
		    placeholder = ft_itoa(code, data);
		    translated = ft_strjoin(translated, placeholder, data);
            i += 2;
        }
        else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
        {
            placeholder2 = ft_strndup(str, i + 1, get_cancer(str, i + 1) - 1, data);
            placeholder = get_env(placeholder2, data->env, data);
            if (placeholder == NULL)
            	placeholder = ft_strdup("", data);
            translated = ft_strjoin(translated, placeholder, data);
            i = get_cancer(str, i + 1);
        }
        else if (str[i] == '$')
        {
            translated = ft_strjoin(translated, "$", data);
            i++;
        }
        else
        {
            translated = ft_strjoin(translated, ft_strndup(str, i, get_dollar(str, i + 1) - 1, data), data);
            i = get_dollar(str, i);
        }
    }
    translated = ft_strjoin(translated, "\n", data);
    //printf("trans :%s\n", translated);
    return (translated);
}

char    *heredoc_expander(char *str, char **file, t_minishell *data)
{
    char    *translated;

    translated = heredoc_dollar(str, data);
    *file = ft_strjoin(*file, translated, data);
    return (*file);
}

char    *manage_heredoc(char *delim, t_minishell *data)
{
    char    *file;
    char    *input;

    file = ft_strdup("", data);
    printf("delimiter : %s\n", delim);
    while (1)
    {
        input = readline("> ");
        if (ft_strcmp(delim, input) == 0)
            break;
        else
            file = heredoc_expander(input, &file, data);
        free(input);
    }
    free(input);
    return (file);
}


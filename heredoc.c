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

char *heredoc_dollar(char *str, t_minishell *data, int j) {
    char *translated = NULL;
    int i = 0;

    translated = ft_strdup("", data);
    while (str[i] != '\0') {
        if (str[i] == '$' && str[i + 1] == '$') {
            translated = process_double_dollar(translated, &i, data);
        } else if (str[i] == '$' && str[i + 1] == '?') {
            translated = process_exit_code(translated, &i, data);
        } else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_')) {
            translated = process_env_variable(str, translated, &i, data);
        } else if (str[i] == '$') {
            translated = process_single_dollar(translated, &i, data);
        } else {
            translated = process_other_chars(str, translated, &i, data);
        }
    }
    if (j != 1)
        translated = ft_strjoin(translated, "\n", data);
    return translated;
}

// Definitions for each helper function
char *process_double_dollar(char *translated, int *i, t_minishell *data) {
    char *placeholder = ft_strdup("$$", data);
    *i += 2;
    return ft_strjoin(translated, placeholder, data);
}

char *process_exit_code(char *translated, int *i, t_minishell *data) {
    int code = data->exit_code;
    char *placeholder = ft_itoa(code, data);
    *i += 2;
    return ft_strjoin(translated, placeholder, data);
}

char *process_env_variable(char *str, char *translated, int *i, t_minishell *data) {
    char *placeholder2 = ft_strndup(str, *i + 1, get_cancer(str, *i + 1) - 1, data);
    char *placeholder = get_env(placeholder2, data->env, data);
    if (placeholder == NULL)
        placeholder = ft_strdup("", data);
    *i = get_cancer(str, *i + 1);
    return ft_strjoin(translated, placeholder, data);
}

char *process_single_dollar(char *translated, int *i, t_minishell *data) {
    *i += 1;
    return ft_strjoin(translated, "$", data);
}

char *process_other_chars(char *str, char *translated, int *i, t_minishell *data) {
    translated = ft_strjoin(translated, ft_strndup(str, *i, get_dollar(str, *i + 1) - 1, data), data);
    *i = get_dollar(str, *i);
    return translated;
}



char    *heredoc_expander(char *str, char **file, t_minishell *data)
{
    char    *translated;

    translated = heredoc_dollar(str, data, 0);
    *file = ft_strjoin(*file, translated, data);
    return (*file);
}

char    *manage_heredoc(char *delim, t_minishell *data)
{
    char    *file;
    char    *input;

    printf("delim : %s\n", delim);
    file = ft_strdup("", data);
    while (1)
    {
        input = readline("> ");
        //printf("delim : %s et input : %s\n", delim, input);
        if (ft_strcmp(delim, heredoc_dollar(input, data, 1)) == 0)
            break;
        else
            file = heredoc_expander(input, &file, data);
        free(input);
    }
    free(input);
    //printf("delim %s\n", file);
    return (file);
}


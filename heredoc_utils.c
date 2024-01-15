/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:34:35 by mdor              #+#    #+#             */
/*   Updated: 2024/01/11 19:34:36 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_dollar(char *str, int i)
{
	while (str[i] != '$' && str[i] != '\0')
		i++;
	return (i);
}

char	*heredoc_dollar(char *str, t_minishell *data, int j)
{
	char	*translated;
	int		i;

	i = 0;
	translated = ft_strdup("", data);
	while (str[i] != '\0')
	{
		if (str[i] == '$' && str[i + 1] == '$')
			translated = process_double_dollar(translated, &i, data);
		else if (str[i] == '$' && str[i + 1] == '?')
			translated = process_code(translated, &i, data);
		else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			translated = process_env_variable(str, translated, &i, data);
		else if (str[i] == '$')
			translated = process_single_dollar(translated, &i, data);
		else
			translated = process_other_chars(str, translated, &i, data);
	}
	if (j != 1)
		translated = ft_strjoin(translated, "\n", data);
	return (translated);
}

char	*process_double_dollar(char *translated, int *i, t_minishell *data)
{
	char	*placeholder;

	placeholder = ft_strdup("$$", data);
	*i += 2;
	return (ft_strjoin(translated, placeholder, data));
}

char	*process_code(char *translated, int *i, t_minishell *data)
{
	int		code;
	char	*placeholder;

	code = data->code;
	placeholder = ft_itoa(code, data);
	*i += 2;
	return (ft_strjoin(translated, placeholder, data));
}

char	*process_env_variable(char *str, char *trans, int *i, t_minishell *data)
{
	char	*placeholder2;
	char	*placeholder;

	placeholder2 = ft_strndup(str, *i + 1, get_cancer(str, *i + 1) - 1, data);
	placeholder = get_env(placeholder2, data->env, data);
	if (placeholder == NULL)
		placeholder = ft_strdup("", data);
	*i = get_cancer(str, *i + 1);
	return (ft_strjoin(trans, placeholder, data));
}

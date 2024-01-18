/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::        */
/*   dollar.c										   :+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2024/01/15 09:36:01 by mdor			  #+#	#+#			      */
/*   Updated: 2024/01/15 09:36:07 by mdor			 ###   ########.fr	      */
/*																			  */
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

void	remove_spaces(char *input_str)
{
	int	read_index;
	int	write_index;
	int	length;

	read_index = 0;
	write_index = 0;
	length = ft_strlen(input_str);
	if (!input_str)
		return ;
	while (input_str[read_index] == ' ')
		read_index++;
	while (read_index < length)
	{
		input_str[write_index++] = input_str[read_index++];
		if (input_str[read_index - 1] == ' ')
		{
			while (input_str[read_index] == ' ' && read_index < length)
				read_index++;
		}
	}
	if (write_index > 0 && input_str[write_index - 1] == ' ')
		write_index--;
	input_str[write_index] = '\0';
}

int	expand_env(char *str, int i, char **expanded, t_minishell *data)
{
	char	*trans;

	trans = get_env(ft_strndup(str, i + 1,
				_next(str, i + 1) - 1, data), data->env, data);
	printf("trans : %s\n", trans);
	if (data->quoted == 0)
		remove_spaces(trans);
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

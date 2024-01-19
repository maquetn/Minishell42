/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:41:15 by nmaquet           #+#    #+#             */
/*   Updated: 2024/01/19 09:29:41 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_variable(t_minishell *data, char *removed_plus)
{
	int		env_count;
	char	**new_env;
	int		i;

	env_count = 0;
	while (data->env[env_count] != NULL)
		env_count++;
	new_env = malloc((env_count + 2) * sizeof(char *));
	i = 0;
	while (i < env_count)
	{
		new_env[i] = no_gc_strdup(data->env[i]);
		i++;
	}
	new_env[i] = no_gc_strdup(removed_plus);
	new_env[i + 1] = NULL;
	i = 0;
	while (i < env_count)
	{
		free(data->env[i]);
		i++;
	}
	free(data->env);
	data->env = new_env;
}

void	update_existing(t_minishell *data, char *current_arg, int env_count)
{
	size_t	current_length;
	size_t	append_length;
	char	*new_buffer;

	new_buffer = NULL;
	if ((ft_strchr(current_arg, '+') != NULL 
			&& syntax_env_var(current_arg) != -1))
	{
		current_length = strlen(data->env[env_count]);
		append_length = strlen(current_arg + syntax_env_var(current_arg) + 1);
		new_buffer = malloc(current_length + append_length + 1);
		ft_strcpy(new_buffer, data->env[env_count]);
		ft_strcat(new_buffer, current_arg + syntax_env_var(current_arg) + 1);
		free(data->env[env_count]);
		data->env[env_count] = new_buffer;
	}
	else
	{
		free(data->env[env_count]);
		data->env[env_count] = no_gc_strdup(current_arg);
	}
}

char	*handle_removed(char *current_arg, t_minishell *data)
{
	char		*char_position;
	size_t		removed_plus_length;
	char		*temp;

	char_position = ft_strchr(current_arg, '+');
	if (char_position != NULL)
	{
		removed_plus_length = char_position - current_arg;
		temp = gc_malloc(sizeof(char) * 
				(removed_plus_length + strlen(char_position + 1) + 1), data);
		ft_strncpy(temp, current_arg, removed_plus_length);
		temp[removed_plus_length] = '\0';
		ft_strncat(temp, char_position + 1, strlen(char_position + 1));
	}
	else
	{
		temp = gc_malloc(sizeof(char) * (strlen(current_arg) + 1), data);
		ft_strcpy(temp, current_arg);
		temp[strlen(current_arg)] = '\0';
	}
	return (temp);
}

void	ft_declare(t_minishell *data, char **args)
{
	int		i;
	char	*equal_sign;

	i = 0;
	if (data && data->env && !args[1])
	{
		while (data->env[i] != NULL)
		{
			equal_sign = ft_strchr(data->env[i], '=');
			write(1, "declare -x ", 11);
			write(1, data->env[i], (equal_sign + 1) - data->env[i]);
			printf("\"%s\"\n", equal_sign + 1);
			++i;
		}
	}
}

void	ft_export(t_minishell *data, char **args)
{
	int		exp;
	char	*current_arg;
	int		env_count;
	char	removed_plus[PATH_MAX];

	exp = 1;
	if (args == NULL)
		return ;
	while (args[exp])
	{
		current_arg = args[exp];
		env_count = 0;
		env_count = simple_equal(data, env_count, current_arg);
		if (data->env[env_count] == NULL && syntax_env_var(current_arg) != -1)
		{
			ft_strcpy(removed_plus, handle_removed(current_arg, data));
			plus_equal(data, 0, removed_plus, current_arg);
		}
		exp++;
	}
	ft_declare(data, args);
}

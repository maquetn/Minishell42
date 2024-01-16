/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:41:15 by nmaquet           #+#    #+#             */
/*   Updated: 2024/01/16 13:43:52 by nmaquet          ###   ########.fr       */
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
		new_env[i] = strdup(data->env[i]);
		i++;
	}
	new_env[i] = strdup(removed_plus);
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
	if ((strchr(current_arg, '+') != NULL && syntax_env_var(current_arg) != -1))
	{
		current_length = strlen(data->env[env_count]);
		append_length = strlen(current_arg + syntax_env_var(current_arg) + 1);
		new_buffer = malloc(current_length + append_length + 1);
		strcpy(new_buffer, data->env[env_count]);
		strcat(new_buffer, current_arg + syntax_env_var(current_arg) + 1);
		free(data->env[env_count]);
		data->env[env_count] = new_buffer;
	}
	else
	{
		free(data->env[env_count]);
		data->env[env_count] = strdup(current_arg);
	}
}

char	*handle_removed(char *current_arg)
{
	char	*char_position;
	size_t	removed_plus_length;
	char	*temp;

	temp = strdup("");
	char_position = strchr(current_arg, '+');
	if (char_position != NULL)
	{
		removed_plus_length = char_position - current_arg;
		strncpy(temp, current_arg, removed_plus_length);
		temp[removed_plus_length] = '\0';
		strcat(temp, char_position + 1);
	}
	else
	{
		temp = malloc((sizeof (char)) * PATH_MAX);
		strncpy(temp, current_arg, PATH_MAX);
		temp[PATH_MAX - 1] = '\0';
	}
	return (temp);
}

void	ft_declare(t_minishell *data, char **args)
{
	int	i;

	i = 0;
	if (data && data->env && !args[1])
	{
		while (data->env[i] != NULL)
		{
			printf("declare -x %s\n", data->env[i]);
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
	while (args[exp])
	{
		current_arg = args[exp];
		env_count = 0;
		env_count = simple_equal(data, env_count, current_arg);
		if (data->env[env_count] == NULL && syntax_env_var(current_arg) != -1)
		{
			strcpy(removed_plus, handle_removed(current_arg));
			plus_equal(data, 0, removed_plus, current_arg);
		}
		exp++;
	}
	ft_declare(data, args);
}

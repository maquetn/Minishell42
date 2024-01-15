/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 14:00:14 by nmaquet           #+#    #+#             */
/*   Updated: 2024/01/15 16:08:06 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	syntax_loop(char *arg, int i)
{
	while (arg[i] && arg[i] != '=')
	{
		if (isalnum(arg[i]) == 0 && arg[i] != '_')
		{
			if (arg[i] == '+' && arg[i + 1] != '=')
			{
				printf("Minishell: export: `%s': not a valid identifier 2 \n", arg);
				return (-1);
			}
		}
		i++;
	}
	return (i);
}

int	syntax_env_var(char *arg)
{
	int	i;

	i = 0;
	if (isdigit(arg[0]) != 0)
	{
		printf("Minishell: export: `%s': not a valid identifier 3 \n", arg);
		return (-1);
	}
	i = syntax_loop(arg, i);
	if (arg[i] == '=')
		return (i);
	else
	{
		printf("Minishell: export: `%s': not a valid identifier 1 \n", arg);
		return (-1);
	}
}

size_t	substr_len(char	*substring, char *equal_sign_pos, char **new_env,
	int i, char *removed_plus)
{
	size_t		substring_length;
	size_t		current_length;
	size_t		append_length;
	size_t		total_length;

	substring_length = equal_sign_pos - new_env[i];
	substring = malloc((substring_length + 1) * sizeof(char));
	current_length = strlen(new_env[i]);
	append_length = strlen(strchr(removed_plus, '=') + 1);
	total_length = current_length + append_length;
	new_env[i] = realloc(new_env[i], total_length + 1);
	strncpy(substring, new_env[i], substring_length);
	substring[substring_length] = '\0';
	return (substring_length);
}

char	**ft_existence(int i, char **new_env, char *removed_plus, int j)
{
	if (j == 0)
		strcat(new_env[i], strchr(removed_plus, '=') + 1);
	else if (j == 1)
	{
		new_env[i] = strdup(removed_plus);
		new_env[i + 1] = NULL;
	}
	return (new_env);
}

int	ft_biggest(size_t substring_length, char *removed_plus, int biggest)
{
	if (substring_length > 0 && substring_length 
		> (strlen(removed_plus) - strlen(strchr(removed_plus, '='))))
		biggest = substring_length;
	else
		biggest = strlen(removed_plus)
			- strlen(strchr(removed_plus, '='));
	return (biggest);
}
/*
void	update_variable(char **new_env, char *removed_plus)
{
	int			i;
	char		*equal_sign_pos;
	int			biggest;
	size_t		substring_length;
	char		*substring;

	i = 0;
	while (new_env[i])
	{
		equal_sign_pos = strchr(new_env[i], '=');
		if (equal_sign_pos != NULL)
		{
			substring_length = substr_len
				(substring, equal_sign_pos, new_env, i, removed_plus);
			biggest = ft_biggest(substring_length, removed_plus, biggest);
			if (strncmp(substring, removed_plus, biggest) == 0)
			{
				ft_existence(i, new_env, removed_plus, 0);
			}
			free(substring);
		}
		i++;
	}
	ft_existence(i, new_env, removed_plus, 1);
}*/

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

int	simple_equal(t_minishell *data, int env_count, char *current_arg)
{
	while (data->env[env_count] != NULL)
	{
		if (syntax_env_var(current_arg) == -1)
			break ;
		if (strncmp(data->env[env_count], current_arg,
				syntax_env_var(current_arg)) == 0
			&& data->env[env_count][syntax_env_var(current_arg)] == '=')
		{
			update_existing(data, current_arg, env_count);
			break ;
		}
		env_count++;
	}
	return (env_count);
}

void	plus_equal(t_minishell *data, int env_count, char *removed_plus,
	char *current_arg)
{
	int	append_flag;

	append_flag = 0;
	while (data->env[env_count] != NULL)
	{
		if (syntax_env_var(removed_plus) == -1)
			break ;
		if (strncmp(data->env[env_count], removed_plus,
				syntax_env_var(removed_plus)) == 0
			&& data->env[env_count]
			[syntax_env_var(removed_plus)] == '=')
		{
			update_existing(data, current_arg, env_count);
			append_flag = 1;
			break ;
		}
		env_count++;
	}
	if (syntax_env_var(removed_plus) != -1)
	{
		env_count++;
		if (append_flag == 0)
		{
			add_variable(data, removed_plus);
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

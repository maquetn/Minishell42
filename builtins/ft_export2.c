/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:34:05 by nmaquet           #+#    #+#             */
/*   Updated: 2024/01/19 10:31:53 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	simple_equal(t_minishell *data, int env_count, char *current_arg)
{
	while (data->env[env_count] != NULL)
	{
		if (syntax_env_var(current_arg, data) == -1)
			break ;
		if (ft_strncmp(data->env[env_count], current_arg,
				syntax_env_var(current_arg, data)) == 0
			&& data->env[env_count][syntax_env_var(current_arg, data)] == '=')
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
		if (syntax_env_var(removed_plus, data) == -1)
			break ;
		if (ft_strncmp(data->env[env_count], removed_plus,
				syntax_env_var(removed_plus, data)) == 0
			&& data->env[env_count]
			[syntax_env_var(removed_plus, data)] == '=')
		{
			update_existing(data, current_arg, env_count);
			append_flag = 1;
			break ;
		}
		env_count++;
	}
	if (syntax_env_var(removed_plus, data) != -1)
	{
		env_count++;
		if (append_flag == 0)
			add_variable(data, removed_plus);
	}
}

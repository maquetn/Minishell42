/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::        */
/*   ft_unset.c										 :+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2024/01/14 14:31:58 by mdor			  #+#	#+#			      */
/*   Updated: 2024/01/14 14:31:59 by mdor			 ###   ########.fr	      */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

unsigned long int	verif_env_var(char *env_var)
{
	int	i;

	i = 0;
	while (env_var[i] && env_var[i] != '=')
		i++;
	if (env_var[i] == '=' && env_var[i + 1])
		return (i);
	else
		return (0);
}

void	ft_unset_loop(t_minishell *data, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		ft_unset(data, args[i]);
		i++;
	}
}

void	ft_unset(t_minishell *data, char *args)
{
	size_t	arg_len;
	int		env_count;

	env_count = 0;
	if (args)
	{
		arg_len = ft_strlen(args);
		while (data->env[env_count] != NULL)
		{
			if (ft_strncmp(data->env[env_count], args,
					verif_env_var(data->env[env_count])) == 0
				&& verif_env_var(data->env[env_count]) == arg_len)
			{
				free(data->env[env_count]);
				while (data->env[env_count + 1] != NULL)
				{
					data->env[env_count] = data->env[env_count + 1];
					env_count++;
				}
				data->env[env_count] = NULL;
				return ;
			}
			env_count++;
		}
	}
}

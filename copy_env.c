/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::        */
/*   copy_env.c										 :+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2024/01/13 10:55:53 by mdor			  #+#	#+#			      */
/*   Updated: 2024/01/13 10:55:55 by mdor			 ###   ########.fr	      */
/*																			  */
/* ************************************************************************** */

//malloc protectd

#include "minishell.h"

char	*increment_sh_lvl(char *str, int i)
{
	char	*shlvl;
	int		lvl;
	char	*lvl_ar;

	shlvl = no_gc_strndup(str, 6, i);
	lvl = ft_atoi(shlvl);
	free(shlvl);
	lvl++;
	lvl_ar = no_gc_itoa(lvl);
	shlvl = no_gc_strjoin("SHLVL=", lvl_ar);
	free(lvl_ar);
	return (shlvl);
}

char	*shlvl_copy(char *str)
{
	int		lvl;
	int		i;
	char	*shlvl;

	i = 6;
	lvl = 0;
	shlvl = NULL;
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]))
			i++;
		else
		{
			lvl = -1;
			break ;
		}
	}
	if (lvl == -1)
		shlvl = no_gc_strdup("SHLVL=1");
	else
	{
		shlvl = increment_sh_lvl(str, i);
	}
	return (shlvl);
}

char	**malloc_new_env_size(char **env)
{
	int		count;
	int		i;
	int		sh_lvl;
	char	**copy;

	i = 0;
	count = 0;
	sh_lvl = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
		{
			i++;
			continue ;
		}
		else if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
			sh_lvl = 1;
		count++;
		i++;
	}
	if (sh_lvl == 0)
		count++;
	return (copy = malloc(sizeof(char *) * (count + 1)));
}

void	process_env_var(EnvProcessArgs *args, int j)
{
	if (ft_strncmp(args->env[j], "OLDPWD=", 7) == 0)
		return ;
	if (ft_strncmp(args->env[j], "SHLVL=", 6) == 0)
	{
		*(args->sh_lvl) = 1;
		(*(args->copy))[++(*(args->i))] = shlvl_copy(args->env[j]);
	}
	else
	{
		(*(args->copy))[++(*(args->i))] = no_gc_strdup(args->env[j]);
	}
}

char	**copy_env(char **env)
{
	char			**copy;
	int				j;
	int				i;
	int				sh_lvl;
	EnvProcessArgs	args;

	i = -1;
	j = -1;
	sh_lvl = 0;
	copy = malloc_new_env_size(env);
	if (!copy)
		return (print_alloc_error());
	args.copy = &copy;
	args.i = &i;
	args.sh_lvl = &sh_lvl;
	args.env = env;
	while (env[++j] != NULL)
		process_env_var(&args, j);
	if (sh_lvl == 0)
		copy[++i] = no_gc_strdup("SHLVL=1");
	copy[++i] = NULL;
	return (copy);
}

// char	**copy_env(char **env)
// {
// 	char	**copy;
// 	int		j;
// 	int		i;
// 	int		sh_lvl;

// 	i = -1;
// 	j = -1;
// 	sh_lvl = 0;
// 	copy = malloc_new_env_size(env);
// 	if (!copy)
// 		return (print_alloc_error());
// 	while (env[++j] != NULL)
// 	{
// 		if (ft_strncmp(env[j], "OLDPWD=", 7) == 0)
// 			continue ;
// 		if (ft_strncmp(env[j], "SHLVL=", 6) == 0)
// 		{
// 			sh_lvl = 1;
// 			copy[++i] = shlvl_copy(env[j]);
// 		}
// 		else
// 			copy[++i] = no_gc_strdup(env[j]);
// 	}
// 	if (sh_lvl == 0)
// 		copy[++i] = no_gc_strdup("SHLVL=1");
// 	copy[++i] = NULL;
// 	return (copy);
// }

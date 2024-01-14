/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 10:55:53 by mdor              #+#    #+#             */
/*   Updated: 2024/01/13 10:55:55 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//malloc protectd

#include "minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s2[i] && s1[i] && s2[i] == s1[i] && i <= n)
	{
		if (s2[i] == s1[i] && i == n - 1)
			return (0);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	long int	famoso;
	int			neg;
	long int	prot;

	famoso = 0;
	neg = 1;
	prot = 0;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		neg *= -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		famoso = famoso * 10 + (*str - 48);
		if (neg == -1 && prot > famoso)
			return (0);
		if (neg == 1 && prot > famoso)
			return (-1);
		prot = famoso;
		str++;
	}
	return (famoso * neg);
}

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

char	**copy_env(char **env)
{
	char	**copy;
	int		j;
	int		i;
	int		sh_lvl;

	i = -1;
	j = -1;
	sh_lvl = 0;
	copy = malloc_new_env_size(env);
	if (!copy)
	{
		ft_putstr_fd("minishell : malloc failure\n", 2);
		return (NULL);
	}
	while (env[++j] != NULL)
	{
		if (ft_strncmp(env[j], "OLDPWD=", 7) == 0)
			continue ;
		if (ft_strncmp(env[j], "SHLVL=", 6) == 0)
		{
			sh_lvl = 1;
			copy[++i] = shlvl_copy(env[j]);
		}
		else
			copy[++i] = no_gc_strdup(env[j]);
	}
	if (sh_lvl == 0)
		copy[++i] = no_gc_strdup("SHLVL=1");
	copy[++i] = NULL;
	return (copy);
}

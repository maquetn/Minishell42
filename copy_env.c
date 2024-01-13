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

char	*shlvl_copy(char *str)
{
	int lvl;
	int	i;
	char	*shlvl = NULL;
	char	*lvl_ar = NULL;

	i = 6;
	lvl = 0;
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
		shlvl = no_gc_strndup(str, 6, i);
		lvl = ft_atoi(shlvl);
		free(shlvl);
		lvl++;
		lvl_ar = no_gc_itoa(lvl);
		shlvl = no_gc_strjoin("SHLVL=", lvl_ar);
		free(lvl_ar);
	}
	return (shlvl);
}

char	**copy_env(char **env)
{
	int		count;
	char	**copy;
	int		i;
	int		old;
	int		sh_lvl;

	count = 0;
	old = 0;
	sh_lvl = 0;
	while (env[count] != NULL)
	{
		if (ft_strncmp(env[count], "OLDPWD=", 7) == 0)
			i = -5;
		if (ft_strncmp(env[count], "SHLVL=", 6) == 0)
			sh_lvl = 1;
		count++;
	}
	if (i == -5 && sh_lvl == 1)
		copy = malloc(sizeof(char *) * (count));
	else if (i == -5 || sh_lvl == 1)
		copy = malloc(sizeof(char *) * (count + 1));
	else
		copy = malloc(sizeof(char *) * (count + 2));
	if (!copy)
	{
		ft_putstr_fd("minishell : malloc failure\n", 2);
		return (NULL);   
	}
	i = -1;
	while (++i < count)
	{
		if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
		{
			old = 1;
			continue;
		}
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
			copy[i] = shlvl_copy(env[i]);
		else if (old == 0)
			copy[i] = strdup(env[i]);
		else
			copy[i - 1] = strdup(env[i]);
	}
	if (sh_lvl == 0)
	{
		copy[i] = no_gc_strdup("SHLVL=1");
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

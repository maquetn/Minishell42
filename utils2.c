/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 16:25:28 by mdor              #+#    #+#             */
/*   Updated: 2024/01/18 19:23:48 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalnum(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	if (c >= 65 && c <= 90)
		return (1);
	if (c >= 97 && c <= 122)
		return (1);
	else
		return (0);
}

int	ft_strcmp(char *s1, char *s2, t_minishell *data)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
	{
		data->error_trigger = 1;
		return (-42000);
	}
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
	return ;
}

char	*ft_itoa(int n, t_minishell *data)
{
	char	*new;
	int		len;
	int		neg;
	long	nbr;

	nbr = n;
	neg = 1;
	if (n < 0)
		neg = 0;
	len = ft_length(nbr);
	new = gc_malloc(sizeof(char) * (len + 1), data);
	if (!new)
		return (NULL);
	new = ft_do(new, len, nbr, neg);
	return (new);
}

char	*ft_strchr(const char *s, int c)
{
	char	*str;

	str = (char *)s;
	while (*str != c)
	{
		if (*str == '\0')
		{
			return (NULL);
		}
		str++;
	}
	return (str);
}

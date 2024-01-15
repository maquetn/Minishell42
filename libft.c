/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 00:50:12 by mdor              #+#    #+#             */
/*   Updated: 2024/01/15 00:50:16 by mdor             ###   ########.fr       */
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

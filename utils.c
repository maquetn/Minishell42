/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:48:02 by mdor              #+#    #+#             */
/*   Updated: 2024/01/12 15:48:03 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_wrdcount(char const *s, char c)
{
	int	i;
	int	count;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	while (*s)
	{
		if (*s != c && i == 0)
		{
			i = 1;
			count++;
		}
		else if (*s == c)
			i = 0;
		s++;
	}
	return (count);
}

char	*ft_dup(const char *s, int start, int end, t_minishell *data)
{
	char	*word;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	word = gc_malloc((end - start + 1) * sizeof(char), data);
	if (!word)
		return (NULL);
	while (start < end)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c, t_minishell *data)
{
	int		iter;
	char	**tabl;
	int		i;
	int		j;

	iter = -1;
	i = -1;
	j = 0;
	tabl = gc_malloc((ft_wrdcount(s, c) + 1) * sizeof(char *), data);
	if (!tabl || !s)
		return (NULL);
	while (++i <= ft_strlen(s))
	{
		if (s[i] != c && iter < 0)
			iter = i;
		else if ((s[i] == c || i == ft_strlen(s)) && iter >= 0)
		{
			tabl[j] = ft_dup(s, iter, i, data);
			if (tabl[j++] == NULL)
				return (NULL);
			iter = -1;
		}
	}
	tabl[j] = 0;
	return (tabl);
}

int	ft_length(long n)
{
	int	len;

	len = 0;
	if (n == 0)
	{
		len++;
		return (len);
	}
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n >= 1)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_do(char *new, int len, long nbr, int neg)
{
	new[len] = '\0';
	if (neg == 0)
		nbr = -nbr;
	while (--len >= 0)
	{
		new[len] = (nbr % 10) + '0';
		nbr /= 10;
	}
	if (neg == 0)
		new[0] = '-';
	return (new);
}

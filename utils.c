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

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char const *s2, t_minishell *data)
{
	int		i;
	int		j;
	char	*new;

	if (!s1 || !s2)
		return (NULL);
	i = ft_strlen((char *)s1);
	j = ft_strlen((char *)s2);
	new = gc_malloc(sizeof(char) * (i + j + 1), data);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		new[j++] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
		new[j++] = s2[i++];
	new[j] = '\0';
	return (new);
}

char	*ft_substr(char const *s, int start, int len, t_minishell *data)
{
	char	*new;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	if (start > ft_strlen(s))
	{
		new = gc_malloc((sizeof(char) * 1), data);
		if (!new)
			return (NULL);
		new[0] = '\0';
		return (new);
	}
	while (i < len && s[start + i])
		i++;
	new = gc_malloc(sizeof(char) * (i + 1), data);
	if (!new)
		return (NULL);
	i = -1;
	while (++i < len && s[start + i])
		new[i] = s[start + i];
	new[i] = '\0';
	return (new);
}

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

char	**ft_free(char **tabl, int i)
{
	int	j;

	j = 0;
	if (tabl)
	{
		while (j < i)
		{
			free(tabl[j]);
			j++;
		}
		free(tabl);
	}
	return (NULL);
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

char	*ft_strdup(const char *s1, t_minishell *data)
{
	char	*a;
	int		len;
	int		i;

	i = 0;
	len = ft_strlen((char *)s1);
	a = gc_malloc(sizeof(char) * (len + 1), data);
	if (!a)
		return (NULL);
	while (i < len)
	{
		a[i] = s1[i];
		i++;
	}
	a[i] = '\0';
	return (a);
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

int	ft_isalpha(int c)
{
	if (c >= 65 && c <= 90)
		return (1);
	if (c >= 97 && c <= 122)
		return (1);
	else
		return (0);
}

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

int	ft_strcmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	if (!s1)
		return (1);
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

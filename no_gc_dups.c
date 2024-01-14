/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::        */
/*   no_gc_dups.c									   :+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2024/01/13 12:35:29 by mdor			  #+#	#+#			      */
/*   Updated: 2024/01/13 12:35:31 by mdor			 ###   ########.fr	      */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

char	*no_gc_strndup(char *str, int start, int end)
{
	int		len;
	char	*dup;
	int		i;

	len = end - start + 1;
	i = 0;
	dup = malloc(sizeof(char) * (len + 1));
	if (dup == NULL)
	{
		ft_putstr_fd("minishell : malloc failure\n", 2);
		return (NULL);
	}
	while (i < len)
	{
		dup[i] = str[start + i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

char	*no_gc_itoa(int n)
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
	new = malloc(sizeof(char) * (len + 1));
	if (!new)
	{
		ft_putstr_fd("minishell : malloc failure\n", 2);
		return (NULL);
	}
	new = ft_do(new, len, nbr, neg);
	return (new);
}

char	*no_gc_strjoin(char *s1, char const *s2)
{
	int		i;
	int		j;
	char	*new;

	if (!s1 || !s2)
		return (NULL);
	i = ft_strlen((char *)s1);
	j = ft_strlen((char *)s2);
	new = malloc(sizeof(char) * (i + j + 1));
	if (!new)
	{
		ft_putstr_fd("minishell : malloc failure\n", 2);
		return (NULL);
	}
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

char	*no_gc_strdup(const char *s1)
{
	char	*a;
	int		len;
	int		i;

	i = 0;
	if (!s1)
		return (NULL);
	len = ft_strlen((char *)s1);
	a = malloc(sizeof(char) * (len + 1));
	if (!a)
	{
		ft_putstr_fd("minishell : malloc failure\n", 2);
		return (NULL);
	}
	while (i < len)
	{
		a[i] = s1[i];
		i++;
	}
	a[i] = '\0';
	return (a);
}

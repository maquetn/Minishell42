/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 16:22:01 by mdor              #+#    #+#             */
/*   Updated: 2024/01/12 16:22:03 by mdor             ###   ########.fr       */
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

int	ft_isalpha(int c)
{
	if (c >= 65 && c <= 90)
		return (1);
	if (c >= 97 && c <= 122)
		return (1);
	else
		return (0);
}

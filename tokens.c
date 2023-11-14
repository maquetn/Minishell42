/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abuchet <abuchet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:17:26 by abuchet           #+#    #+#             */
/*   Updated: 2023/11/14 15:41:21 by abuchet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strslen(char **strs)
{
	int i;

	i = 0;
	while(strs[i])
		i++;
	return (i);
}

void ft_arg(char **strs, int i)
{
	int max_len = ft_strslen(strs);
	int j = ft_strlen(strs[i]);
	int d = j - 2;
	int n = 0;
	
	while(max_len > i + 1)
	{
		strs[max_len + 1] = strs[max_len];
		max_len--;
	}

	while (j > d)
	{
		strs[max_len][n++] = strs[i][j];
		j--;
	}
}

int main()
{
	char *str = "hello>>world";
	char **strs;
	int i = 0;
	int j = 0;

	strs = ft_split(str, ' ');
	while (strs[i])
	{
		while (strs[i][j])
		{
			if (strs[i][j] == '|' || strs[i][j] == '>' || strs[i][j] == '<')
			{
				if ((strs[i][j] == '>' && strs[i][j + 1] == '>') || 
					(strs[i][j] == '<' && strs[i][j + 1] == '<'))
					{
						ft_arg(strs, i);
						j++;
					}
					
			}
			j++;
		}
		j = 0;
		i++;
	}
	i = 0;
	while(strs[i])
	{
		printf("%s\n", strs[i]);
		i++;
	}
}
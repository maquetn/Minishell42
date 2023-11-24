/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:46:43 by abuchet           #+#    #+#             */
/*   Updated: 2023/11/24 16:15:31 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strncpy(char *dst, const char *src, int size)
{
	int	len;

	len = 0;
	if (size == 0)
	{
		while (src[len])
			len++;
		return (len);
	}
	while (len < size - 1 && src[len])
	{
		dst[len] = src[len];
		len++;
	}
	if (len < size)
		dst[len] = '\0';
	while (src[len])
		len++;
	return (len);
}
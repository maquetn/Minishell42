/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abuchet <abuchet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:46:43 by abuchet           #+#    #+#             */
/*   Updated: 2023/11/14 15:47:00 by abuchet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

size_t	ft_strncpy(char *dst, const char *src, size_t size)
{
	size_t	len;

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
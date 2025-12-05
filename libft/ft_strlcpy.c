/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:42:21 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/17 16:18:17 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Copy up to size - 1 characters from src into dst and ensure dst is null-terminated when size is greater than zero.
 *
 * @param dst Destination buffer where the string will be copied.
 * @param src Source NUL-terminated string to copy from.
 * @param size Size of the destination buffer in bytes; if zero, nothing is written to dst.
 * @returns The total length of src (the number of bytes in src not including the terminating NUL).
 */
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (src[len] != '\0')
		len++;
	if (size == 0)
		return (len);
	while (i < size - 1 && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}
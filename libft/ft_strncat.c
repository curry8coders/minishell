/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 01:18:10 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/29 03:07:39 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Append up to n characters from src to the end of dst.
 *
 * The copy stops when n characters have been appended or when a null byte
 * is encountered in src; dst is null-terminated after the operation.
 * This function does not check destination buffer capacity and may overflow
 * dst if it is not large enough to hold the result.
 *
 * @param dst Destination buffer to which characters are appended.
 * @param src Source string from which characters are taken.
 * @param n Maximum number of characters to append from src.
 * @returns Pointer to dst (the resulting concatenated string).
 */
char	*ft_strncat(char *dst, const char *src, size_t n)
{
	size_t	len_dst;
	size_t	i;

	len_dst = 0;
	i = 0;
	while (dst[len_dst] != '\0')
		len_dst++;
	while (i < n && src[i] != '\0')
	{
		dst[len_dst + i] = src[i];
		i++;
	}
	dst[len_dst + i] = '\0';
	return (dst);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 03:20:40 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 02:33:19 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Append `src` to the end of `dst` without exceeding a total buffer size and report
 * the total length the function attempted to create.
 *
 * @param dst Destination buffer where `src` will be appended; must be NUL-terminated if its
 *            length is less than `size`.
 * @param src NUL-terminated source string to append to `dst`.
 * @param size Total size of the destination buffer `dst`.
 * @returns The sum of the initial length of `dst` (up to `size`) and the length of `src`;
 *          this is the length of the string the function tried to create. If `size` is
 *          less than or equal to the initial length of `dst`, no bytes are appended and
 *          the return value is `size + strlen(src)`.
 */
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len_dst;
	size_t	len_src;

	i = 0;
	len_dst = 0;
	len_src = 0;
	while (dst[len_dst] != '\0' && len_dst < size)
		len_dst++;
	while (src[len_src] != '\0')
		len_src++;
	while (size <= len_dst)
		return (size + len_src);
	while (src[i] != '\0' && (len_dst + i) < (size - 1))
	{
		dst[len_dst + i] = src[i];
		i++;
	}
	dst[len_dst + i] = '\0';
	return (len_dst + len_src);
}
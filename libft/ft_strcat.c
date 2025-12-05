/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 01:18:10 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/29 03:35:27 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Append the string `src` to the end of `dst`.
 *
 * @param dst Destination buffer which must be large enough to hold the resulting string and must be null-terminated.
 * @param src Source null-terminated string to append.
 * @returns Pointer to `dst` containing the concatenated string.
 */
char	*ft_strcat(char *dst, const char *src)
{
	size_t	len_dst;
	size_t	len_src;

	len_dst = 0;
	len_src = 0;
	while (dst[len_dst] != '\0')
		len_dst++;
	while (src[len_src] != '\0')
	{
		dst[len_dst + len_src] = src[len_src];
		len_src++;
	}
	dst[len_dst + len_src] = '\0';
	return (dst);
}
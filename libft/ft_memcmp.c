/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 07:32:04 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/17 15:38:09 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Compare up to `n` bytes of two memory areas.
 *
 * Comparison is performed byte-wise using unsigned byte values.
 *
 * @param s1 Pointer to the first memory area to compare.
 * @param s2 Pointer to the second memory area to compare.
 * @param n  Number of bytes to compare.
 * @returns A negative value if the first differing byte in `s1` is less than that in `s2`, a positive value if it is greater, or `0` if the first `n` bytes are identical.
 */
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*p1;
	const unsigned char	*p2;
	size_t				i;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (p1[i] != p2[i])
			return ((int)(p1[i] - p2[i]));
		i++;
	}
	return (0);
}
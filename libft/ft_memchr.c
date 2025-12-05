/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 04:38:54 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 02:48:05 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Locate the first occurrence of a byte value within a memory block.
 *
 * @param s Pointer to the memory area to be searched.
 * @param c Byte value to search for (converted to `unsigned char`).
 * @param n Number of bytes to examine starting at `s`.
 * @returns Pointer to the first matching byte within `s`, or `NULL` if no match is found within the first `n` bytes.
 */
void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr;
	unsigned char		target;
	size_t				i;

	ptr = (const unsigned char *)s;
	target = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == target)
			return ((void *)(ptr + i));
		i++;
	}
	return (NULL);
}
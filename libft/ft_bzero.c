/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 03:38:21 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/14 17:45:05 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Set the first n bytes of the memory area pointed to by s to zero.
 *
 * Zeros out the first n bytes of the memory region starting at s. If n is 0,
 * the function performs no action.
 *
 * @param s Pointer to the memory region to be zeroed.
 * @param n Number of bytes to set to zero.
 */
void	ft_bzero(void *s, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (0 < n)
	{
		*p++ = '\0';
		n--;
	}
}
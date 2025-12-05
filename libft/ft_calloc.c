/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:04:19 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/26 01:40:59 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

/**
 * Allocate zero-initialized memory for an array of `nmemb` elements of `size` bytes each.
 *
 * @param nmemb Number of elements to allocate.
 * @param size Size in bytes of each element.
 * @returns Pointer to the zero-initialized memory for the array, or `NULL` if allocation fails or if `nmemb * size` would overflow. If `nmemb` or `size` is zero, returns the result of `malloc(0)`.
 */
void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	void	*ptr;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	total_size = nmemb * size;
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total_size);
	return (ptr);
}
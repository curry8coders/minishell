/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:04:19 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/24 00:20:26 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

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

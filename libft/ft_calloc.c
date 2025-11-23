/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:04:19 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/28 16:22:06 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "libft.h"
#include <limits.h>

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

#include <stdio.h>

int main(void)
{
    void *ptr;

	ptr = ft_calloc(SIZE_MAX, 2);
    if (!ptr)
        printf("Overflow check passed. Allocation failed as expected.\n");

    return 0;
}

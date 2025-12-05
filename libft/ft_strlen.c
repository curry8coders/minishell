/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:00:16 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 11:58:49 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Calculate the length of a null-terminated C string.
 *
 * @param c Pointer to the null-terminated string to measure. Behavior is undefined if `c` is NULL.
 * @returns The number of characters preceding the terminating null byte.
 */
size_t	ft_strlen(const char *c)
{
	size_t	i;

	i = 0;
	while (*c++ != '\0')
		i++;
	return (i);
}
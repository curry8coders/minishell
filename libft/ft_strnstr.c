/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 07:39:36 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 02:51:56 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Locate the first occurrence of the substring `little` within the first `len` bytes of `big`.
 *
 * If `little` is an empty string, returns `big`.
 *
 * @param big String to be searched.
 * @param little Substring to locate in `big`.
 * @param len Maximum number of bytes of `big` to search.
 * @returns Pointer to the first occurrence of `little` in `big` within the first `len` bytes, or `NULL` if no such occurrence exists.
 */
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	while (big[i] != '\0' && i < len)
	{
		j = 0;
		while (((i + j) < len) && (big[i + j] == little[j]))
		{
			if (little[j + 1] == '\0')
				return ((char *)&big[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:16:23 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 02:44:42 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Locate the first occurrence of a character in a C string.
 *
 * @param s String to search.
 * @param c Integer value of the character to locate; converted to `char` for comparison.
 * @returns Pointer to the first matching character within `s`; if `c` is `'\0'` returns a pointer to the terminating null byte; `NULL` if the character is not found.
 */
char	*ft_strchr(const char *s, int c)
{
	char	target;

	target = (char)c;
	while (*s != '\0')
	{
		if (*s == target)
			return ((char *)s);
		s++;
	}
	if (target == '\0')
		return ((char *)s);
	return (NULL);
}
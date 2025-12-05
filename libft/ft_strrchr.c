/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 21:31:02 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 02:52:23 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Locate the last occurrence of a character in a null-terminated string.
 *
 * @param s The null-terminated string to search.
 * @param c The character to locate (interpreted as an unsigned char converted to `char`).
 * @returns Pointer to the last occurrence of `c` within `s`, or `NULL` if `c` is not found.
 *          If `c` is '\\0', returns a pointer to the terminating null byte of `s`.
 */
char	*ft_strrchr(const char *s, int c)
{
	char	target;
	char	*last_occurrence;

	target = (char)c;
	last_occurrence = NULL;
	while (*s != '\0')
	{
		if (*s == target)
			last_occurrence = (char *)s;
		s++;
	}
	if (target == '\0')
		return ((char *)s);
	return (last_occurrence);
}
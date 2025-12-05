/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:05:43 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 02:40:08 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Copy every second character from `src` into `dest` and return a pointer to the end of the written data.
 *
 * The function reads `src` and writes characters at source indices 1, 3, 5, ... into `dest` until a source character tested for termination is `'\0'`. It does not write a terminating `'\0'` into `dest` and does not perform null-pointer checks.
 *
 * @param dest Destination buffer where selected characters are written.
 * @param src  Source C-string to read characters from.
 * @returns Pointer to the position in `dest` immediately after the last written character.
 */
char	*ft_strcpy(char *dest, const char *src)
{
	while (*src++ != '\0')
		*dest++ = *src++;
	return (dest);
}

/**
 * Allocate and return a heap-allocated duplicate of a null-terminated string.
 *
 * The returned buffer is a newly allocated copy of `s`; the caller is
 * responsible for freeing it. Behavior is undefined if `s` is `NULL`.
 *
 * @param s Null-terminated string to duplicate.
 * @returns Pointer to the newly allocated duplicate, or `NULL` if memory allocation fails.
 */
char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*dup;

	dup = (char *) malloc(ft_strlen(s) + 1);
	if (!dup)
		return (NULL);
	len = 0;
	while (s[len])
	{
		dup[len] = s[len];
		len++;
	}
	dup[len] = '\0';
	return (dup);
}
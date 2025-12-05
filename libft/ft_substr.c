/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:35:34 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 10:12:21 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Create a newly allocated string containing up to `len` characters from `s`,
 * starting at index `start`.
 *
 * @param s Source string. If `s` is NULL the function returns NULL.
 * @param start Starting index in `s`. If `start` is greater than the length of `s`,
 *              an empty string is returned.
 * @param len Maximum number of characters to include from `s` beginning at `start`;
 *            if `len` exceeds the remaining characters it is truncated to the
 *            available length.
 * @returns Pointer to a null-terminated allocated substring, an empty string if
 *          `start` is past the end of `s`, or NULL if `s` is NULL or allocation fails.
 */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	return (str);
}
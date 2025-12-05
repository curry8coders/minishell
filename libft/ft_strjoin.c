/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:11:08 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 10:13:58 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Allocate and return a new string containing s1 followed by s2.
 *
 * The returned string is null-terminated and must be freed by the caller.
 * Behavior is undefined if `s1` or `s2` is NULL.
 *
 * @param s1 Null-terminated prefix string.
 * @param s2 Null-terminated suffix string.
 * @returns Pointer to the newly allocated concatenated string, or `NULL` if allocation fails.
 */
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined_str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	joined_str = (char *) malloc((ft_strlen(s1) + ft_strlen(s2) + 1)
			* sizeof(char));
	if (!joined_str)
		return (NULL);
	while (s1[i])
		joined_str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		joined_str[j++] = s2[i++];
	joined_str[j] = 0;
	return (joined_str);
}
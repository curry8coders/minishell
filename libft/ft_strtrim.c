/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:30:46 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 10:15:00 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Check whether a character is contained in a null-terminated character set.
 *
 * @param c Character to search for.
 * @param set Null-terminated string representing the set of characters to check against.
 * @returns `1` if `c` is found in `set`, `0` otherwise.
 */
int	is_in_set(char c, const char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

/**
 * Create a newly allocated string with characters from `set` removed from the start and end of `s1`.
 *
 * @param s1 The input null-terminated string to trim. If NULL, the function returns NULL.
 * @param set A null-terminated string containing characters to remove from both ends of `s1`. If NULL, the function returns NULL.
 * @returns Pointer to a newly allocated null-terminated string containing the trimmed result, an empty string if all characters are removed, or NULL on invalid input or allocation failure.
 */
char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed_str;
	size_t	i;

	start = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[start] && is_in_set(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && is_in_set(s1[end - 1], set))
		end--;
	trimmed_str = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!trimmed_str)
		return (NULL);
	i = 0;
	while (start < end)
		trimmed_str[i++] = s1[start++];
	trimmed_str[i] = '\0';
	return (trimmed_str);
}
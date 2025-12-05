/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 22:47:38 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 11:20:14 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Counts the words in a null-terminated string separated by the specified delimiter.
 *
 * @param s Null-terminated input string to examine.
 * @param c Delimiter character used to separate words.
 * @returns The number of non-empty substrings in `s` separated by `c`.
 */
static size_t	ft_wordcount(const char *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

/**
 * Duplicate the substring of `s` between indices `start` (inclusive) and `end` (exclusive).
 *
 * @param s Pointer to the source null-terminated string.
 * @param start Zero-based start index of the substring (inclusive).
 * @param end Zero-based end index of the substring (exclusive).
 * @returns A newly allocated null-terminated string containing the specified substring,
 *          or `NULL` if memory allocation fails.
 */
static char	*ft_worddup(const char *s, size_t start, size_t end)
{
	char	*word;
	size_t	i;

	i = 0;
	word = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!word)
		return (NULL);
	while (start < end)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

/**
 * Free `k` allocated strings stored in `res` and then free the `res` array itself.
 *
 * Frees res[k-1] down to res[0] (if k > 0) and finally frees the pointer array `res`.
 *
 * @param res Null-terminated array of strings or array of pointers previously allocated.
 * @param k Number of elements in `res` to free; only the first `k` entries are freed.
 */
void	free_place(char **res, size_t k)
{
	while (k--)
		free(res[k]);
	free(res);
}

/**
 * Populate a preallocated array with substrings of `s` split by delimiter `c`.
 *
 * Fills `res` with newly allocated null-terminated strings for each non-empty
 * token found in `s`, and terminates the array with a NULL pointer.
 *
 * @param res Preallocated array of `char *` with capacity for all tokens plus a terminating NULL.
 * @param s Input string to split.
 * @param c Delimiter character.
 *
 * @returns `res` on success, `NULL` if a substring allocation fails (in which case any previously
 * allocated entries are freed).
static	char	**split_words(char **res, const char *s, char c)
{
	size_t	i;
	size_t	k;
	size_t	start;

	i = 0;
	k = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			res[k++] = ft_worddup(s, start, i);
			if (!res[k - 1])
			{
				free_place(res, k - 1);
				return (NULL);
			}
		}
	}
	res[k] = NULL;
	return (res);
}

/**
 * Split a string into substrings delimited by a specified character.
 *
 * @param s The null-terminated string to split. If `s` is NULL, the function returns NULL.
 * @param c The delimiter character that separates substrings.
 * @returns An array of null-terminated strings containing the substrings of `s` separated by `c`, terminated by a NULL pointer; returns NULL if `s` is NULL or if memory allocation fails.
 */
char	**ft_split(char const *s, char c)
{
	char	**res;

	if (!s)
		return (NULL);
	res = malloc(sizeof(char *) *(ft_wordcount(s, c) + 1));
	if (!res)
		return (NULL);
	return (split_words(res, s, c));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/29 14:59:29 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

#include <stdio.h>
#include <string.h>

/**
 * Check whether a null-terminated string is a valid identifier:
 * the first character must be an alphabetic character or underscore; subsequent characters must be alphabetic, numeric, or underscore.
 * @param s String to validate as an identifier.
 * @returns `true` if `s` is a valid identifier, `false` otherwise.
 */
bool	is_identifier(const char *s)
{
	if (!is_alpha_under(*s))
		return (false);
	s++;
	while (*s)
	{
		if (!is_alpha_num_under(*s))
			return (false);
		s++;
	}
	return (true);
}

/**
 * Create and initialize a new t_item with the provided name and value.
 *
 * Allocates and zero-initializes a t_item, sets its `name` and `value`
 * fields to the provided pointers. Ownership of `name` and `value`
 * is transferred to the returned item. On allocation failure this
 * function calls fatal_error.
 *
 * @param name Null-terminated string to assign as the item's name (ownership transferred).
 * @param value Null-terminated string to assign as the item's value, or NULL (ownership transferred).
 * @returns Pointer to the newly allocated t_item.
 */
t_item	*item_new(char *name, char *value)
{
	t_item	*item;

	item = calloc(1, sizeof(*item));
	if (item == NULL)
		fatal_error("calloc");
	item->name = name;
	item->value = value;
	return (item);
}

/**
 * Create a newly allocated string representation of an item in the form "name" or "name=value".
 * @param item Item whose name (and optional value) will be formatted; `item->name` must be non-NULL.
 * @returns Newly allocated null-terminated string containing "name" or "name=value". Caller is responsible for freeing the returned buffer.
 */
char	*item_get_string(t_item *item)
{
	size_t	strsize;
	char	*string;

	strsize = strlen(item->name) + 2;
	if (item->value)
		strsize += strlen(item->value);
	string = malloc(strsize);
	if (string == NULL)
		fatal_error("malloc");
	ft_strlcpy(string, item->name, strsize);
	if (item->value)
	{
		ft_strlcat(string, "=", strsize);
		ft_strlcat(string, item->value, strsize);
	}
	return (string);
}

/**
 * Insert or update a map entry from a "name=value" formatted string.
 *
 * Parses `string` into a name and optional value and stores the pair in `map`.
 *
 * @param map Target map to modify.
 * @param string Input text in the form "name=value" or "name".
 * @param allow_empty_value When false, reject inputs that do not contain '='.
 * @returns `-1` if `string` lacks '=' and `allow_empty_value` is false; otherwise the result code from the map insertion operation.
 */
int	map_put(t_map *map, const char *string, bool allow_empty_value)
{
	int		result;
	char	*name;
	char	*value;

	if (!allow_empty_value && strchr(string, '=') == NULL)
		return (-1);
	parse_name_value(string, &name, &value);
	result = map_set(map, name, value);
	free(name);
	free(value);
	return (result);
}

/**
 * Count items stored in a map.
 *
 * @param map Pointer to the map whose items will be counted.
 * @param count_null_value If `true`, include items whose `value` is NULL; if `false`, count only items with a non-NULL `value`.
 * @returns The number of items in the map matching the counting rule.
 */
size_t	map_len(t_map *map, bool count_null_value)
{
	size_t	len;
	t_item	*item;

	len = 0;
	item = map->item_head.next;
	while (item)
	{
		if (item->value || count_null_value)
			len++;
		item = item->next;
	}
	return (len);
}
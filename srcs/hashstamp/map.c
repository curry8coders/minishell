/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 04:24:42 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/29 14:51:42 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

#include <stdio.h>
#include <string.h>

/**
 * Allocate and return a new, zero-initialized t_map.
 *
 * On allocation failure this function calls fatal_error("calloc") (does not return).
 *
 * @returns Pointer to a newly allocated, zero-initialized `t_map`.
 */
t_map	*map_new(void)
{
	t_map	*map;

	map = calloc(1, sizeof(*map));
	if (map == NULL)
		fatal_error("calloc");
	return (map);
}

char	*map_get(t_map *map, const char *name)
{
	t_item	*cur;

	if (name == NULL)
		return (NULL);
	cur = map->item_head.next;
	while (cur)
	{
		if (strcmp(cur->name, name) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

/**
 * Remove a named entry from the map.
 *
 * If an item with the given name exists, it is unlinked from the map and its
 * name and value buffers and item structure are freed.
 *
 * @param map Map from which to remove the item.
 * @param name Name of the item to remove.
 * @returns `-1` if `name` is NULL or is not a valid identifier; `0` if the operation completed
 *          (item was removed or no matching item was found).
 */
int	map_unset(t_map *map, const char *name)
{
	t_item	*cur;
	t_item	*prev;

	if (name == NULL || !is_identifier(name))
		return (-1);
	prev = &map->item_head;
	cur = map->item_head.next;
	while (cur)
	{
		if (strcmp(cur->name, name) == 0)
		{
			prev->next = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return (0);
		}
		prev = prev->next;
		cur = cur->next;
	}
	return (0);
}

/**
 * Insert a new entry into the map or update the value of an existing entry.
 *
 * @param map Map to modify.
 * @param name Identifier name for the entry; must be a valid identifier.
 * @param value String value to associate with the name (may be NULL depending on use).
 * @returns `0` on success; `-1` if `name` is NULL or not a valid identifier.
 */
int	map_set(t_map *map, const char *name, const char *value)
{
	t_item	*cur;

	if (name == NULL || !is_identifier(name))
		return (-1);
	cur = map->item_head.next;
	while (cur)
	{
		if (strcmp(cur->name, name) == 0)
			break ;
		cur = cur->next;
	}
	if (cur)
		update_existing_item(cur, value);
	else
	{
		cur = create_new_item(name, value);
		cur->next = map->item_head.next;
		map->item_head.next = cur;
	}
	return (0);
}
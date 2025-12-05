/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_item.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/29 14:59:29 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

#include <string.h>

/**
 * Update an item's value to a copy of the provided string or NULL.
 *
 * Frees the existing value stored in `cur->value` and sets it to a newly
 * allocated duplicate of `value`; if `value` is NULL, `cur->value` is set to
 * NULL. Calls `fatal_error("strdup")` if memory allocation for the duplicate fails.
 *
 * @param cur Item whose value will be replaced (must be non-NULL).
 * @param value New value string to duplicate, or NULL to clear the value.
 */
void	update_existing_item(t_item *cur, const char *value)
{
	free(cur->value);
	if (value == NULL)
		cur->value = NULL;
	else
	{
		cur->value = strdup(value);
		if (cur->value == NULL)
			fatal_error("strdup");
	}
}

/**
 * Create a new t_item duplicating the provided name and optional value.
 *
 * @param name Null-terminated string for the item's name (duplicated).
 * @param value Optional null-terminated string for the item's value; pass NULL to create an item with no value.
 * @returns Pointer to the newly allocated t_item with duplicated name and value (value may be NULL).
 * @note Calls fatal_error("strdup") if duplicating the name or value fails.
 */
t_item	*create_new_item(const char *name, const char *value)
{
	t_item	*cur;

	if (value == NULL)
	{
		cur = item_new(strdup(name), NULL);
		if (cur->name == NULL)
			fatal_error("strdup");
	}
	else
	{
		cur = item_new(strdup(name), strdup(value));
		if (cur->name == NULL || cur->value == NULL)
			fatal_error("strdup");
	}
	return (cur);
}

/**
 * Split an input string at the first '=' into a newly allocated name and an optional value.
 *
 * @param string Input string in the form "name" or "name=value".
 * @param name Out parameter set to a newly allocated string containing the substring before the first '='; caller must free it.
 * @param value Out parameter set to a newly allocated string containing the substring after the first '=', or set to NULL if no '=' was present; caller must free it when non-NULL.
 *
 * On allocation failure, the function calls fatal_error("strdup").
 */
void	parse_name_value(const char *string, char **name, char **value)
{
	char	*name_end;

	name_end = strchr(string, '=');
	if (name_end == NULL)
	{
		*name = strdup(string);
		*value = NULL;
		if (*name == NULL)
			fatal_error("strdup");
	}
	else
	{
		*name = strndup(string, name_end - string);
		*value = strdup(name_end + 1);
		if (*name == NULL || *value == NULL)
			fatal_error("strdup");
	}
}
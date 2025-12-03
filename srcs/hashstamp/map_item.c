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

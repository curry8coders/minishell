/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/05 21:37:32 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_item.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/05 21:37:32 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "minishell.h"

void	update_existing_item(t_item *cur, const char *value)
{
	free(cur->value);
	if (value == NULL)
		cur->value = NULL;
	else
	{
		cur->value = ft_strdup(value);
		if (cur->value == NULL)
			fatal_error("ft_strdup");
	}
}

t_item	*create_new_item(const char *name, const char *value)
{
	t_item	*cur;

	if (value == NULL)
	{
		cur = item_new(ft_strdup(name), NULL);
		if (cur->name == NULL)
			fatal_error("ft_strdup");
	}
	else
	{
		cur = item_new(ft_strdup(name), ft_strdup(value));
		if (cur->name == NULL || cur->value == NULL)
			fatal_error("ft_strdup");
	}
	return (cur);
}

void	parse_name_value(const char *string, char **name, char **value)
{
	char	*name_end;

	name_end = ft_strchr(string, '=');
	if (name_end == NULL)
	{
		*name = ft_strdup(string);
		*value = NULL;
		if (*name == NULL)
			fatal_error("ft_strdup");
	}
	else
	{
		*name = ft_substr(string, 0, name_end - string);
		*value = ft_strdup(name_end + 1);
		if (*name == NULL || *value == NULL)
			fatal_error("ft_strdup");
	}
}

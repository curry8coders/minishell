/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 11:54:25 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/29 11:25:00 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>

void	print_allenv(void)
{
	t_item	*cur;

	cur = g_envmap->item_head.next;
	while (cur)
	{
		if (cur->value)
			printf("declare -x %s=\"%s\"\n", cur->name, cur->value);
		else
			printf("declare -x %s\n", cur->name);
		cur = cur->next;
	}
}

int	builtin_export(char **argv)
{
	size_t	i;
	int		status;

	if (argv[1] == NULL)
	{
		print_allenv();
		return (0);
	}
	status = 0;
	i = 1;
	while (argv[i])
	{
		if (map_put(g_envmap, argv[i], true) < 0)
		{
			builtin_error("export", argv[i], "not a valid identifier");
			status = 1;
		}
		i++;
	}
	return (status);
}

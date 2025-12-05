/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 13:46:09 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/29 11:24:47 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>

/**
 * Print the current environment variables to standard output.
 *
 * Iterates the global environment map and writes each entry with a non-null value
 * in the form "name=value" on its own line, then prints the fixed entry
 * "_=/usr/bin/env".
 *
 * @param argv Ignored; accepted for builtin signature compatibility.
 * @return 0 on success.
 */
int	builtin_env(char **argv)
{
	t_item	*cur;

	(void)argv;
	cur = g_envmap->item_head.next;
	while (cur)
	{
		if (cur->value)
			printf("%s=%s\n", cur->name, cur->value);
		cur = cur->next;
	}
	printf("_=/usr/bin/env\n");
	return (0);
}
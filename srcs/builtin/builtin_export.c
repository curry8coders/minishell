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

/**
 * Output all environment variables in bash-compatible `declare -x` format to stdout.
 *
 * For each entry in the global `g_envmap` list, prints `declare -x NAME="VALUE"` when a value exists,
 * otherwise prints `declare -x NAME`.
 */
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

/**
 * Handle the `export` builtin: either print all environment variables or add arguments to the environment.
 *
 * If no arguments are provided after the command, prints all environment variables in the shell's export format.
 * Otherwise attempts to add each argument as an environment entry; invalid identifiers are reported and cause
 * a non-zero exit status.
 *
 * @param argv NULL-terminated argument vector where argv[0] is the command name and subsequent entries are
 *             the values to export.
 * @returns `0` if all provided arguments were valid (or when printing the environment), `1` if any argument
 *          was not a valid identifier.
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 13:39:01 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/29 11:25:12 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Remove the environment variables named in argv from the global environment map.
 *
 * For each argument starting at argv[1], attempts to remove that name from the
 * global g_envmap. If removal fails for an argument, an error is reported via
 * builtin_error with message "not a valid identifier".
 *
 * @param argv NULL-terminated argument vector where argv[0] is the command name;
 *             processing begins at argv[1].
 * @returns `0` if the final processed argument was removed successfully or if
 *          no arguments were provided, `1` if the final processed argument was
 *          an invalid identifier (an error is reported in that case).
 */
int	builtin_unset(char **argv)
{
	int		status;
	size_t	i;

	status = 0;
	i = 1;
	while (argv[i])
	{
		if (map_unset(g_envmap, argv[i]) < 0)
		{
			builtin_error("unset", argv[i], "not a valid identifier");
			status = 1;
		}
		else
			status = 0;
		i++;
	}
	return (status);
}
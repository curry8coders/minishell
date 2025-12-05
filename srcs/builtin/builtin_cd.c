/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 14:04:45 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/30 20:47:09 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include "minishell.h"

#include <string.h>

/**
 * Determine the target directory path for the cd builtin and write it into the provided buffer.
 *
 * If no argument is provided in argv, the function uses the HOME environment variable as the target;
 * otherwise it uses argv[1]. The chosen path is copied into the caller-provided buffer.
 *
 * @param path Buffer (size PATH_MAX) where the target path will be stored.
 * @param argv Argument vector passed to the builtin; argv[1] is used when present.
 * @returns `0` on success, `1` if no argument was given and the HOME environment variable is not set.
 */
static int	get_target_path(char *path, char **argv)
{
	char	*home;

	if (argv[1] == NULL)
	{
		home = map_get(g_envmap, "HOME");
		if (home == NULL)
		{
			builtin_error("cd", NULL, "HOME not set");
			return (1);
		}
		ft_strlcpy(path, home, PATH_MAX);
	}
	else
		ft_strlcpy(path, argv[1], PATH_MAX);
	return (0);
}

/**
 * Change the current working directory and update `PWD` and `OLDPWD` in the global environment map.
 *
 * @param argv Argument vector for the `cd` command; `argv[1]` is the optional target directory.
 *            If `argv[1]` is NULL, the function attempts to use the `HOME` value from `g_envmap`.
 *
 * @returns `0` on success, `1` on failure (for example if `HOME` is not set when no target is given, or if `chdir` fails).
 */
int	builtin_cd(char **argv)
{
	char	*oldpwd;
	char	path[PATH_MAX];
	char	*newpwd;

	oldpwd = map_get(g_envmap, "PWD");
	map_set(g_envmap, "OLDPWD", oldpwd);
	if (get_target_path(path, argv) != 0)
		return (1);
	if (chdir(path) < 0)
	{
		builtin_error("cd", NULL, "chdir");
		return (1);
	}
	newpwd = resolve_pwd(oldpwd, path);
	map_set(g_envmap, "PWD", newpwd);
	free(newpwd);
	return (0);
}
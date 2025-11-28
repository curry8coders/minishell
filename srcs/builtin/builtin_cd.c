/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 14:04:45 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/28 22:50:53 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>
#include <linux/limits.h>
#include <unistd.h>
#include "minishell.h"

#include <string.h>

static int	get_target_path(char *path, char **argv)
{
	char	*home;

	if (argv[1] == NULL)
	{
		home = map_get(envmap, "HOME");
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

int	builtin_cd(char **argv)
{
	char	*oldpwd;
	char	path[PATH_MAX];
	char	*newpwd;

	oldpwd = map_get(envmap, "PWD");
	map_set(envmap, "OLDPWD", oldpwd);
	if (get_target_path(path, argv) != 0)
		return (1);
	if (chdir(path) < 0)
	{
		builtin_error("cd", NULL, "chdir");
		return (1);
	}
	newpwd = resolve_pwd(oldpwd, path);
	map_set(envmap, "PWD", newpwd);
	free(newpwd);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 14:04:45 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/05 22:35:45 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "minishell.h"

static int	get_target_path(t_shell *sh, char *path, char **argv)
{
	char	*home;

	if (argv[1] == NULL)
	{
		home = map_get(sh->envmap, "HOME");
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

int	builtin_cd(t_shell *shell, char **argv)
{
	char	*oldpwd;
	char	path[PATH_MAX];
	char	*newpwd;

	oldpwd = map_get(shell->envmap, "PWD");
	map_set(shell->envmap, "OLDPWD", oldpwd);
	if (get_target_path(shell, path, argv) != 0)
		return (1);
	if (chdir(path) < 0)
	{
		builtin_error("cd", argv[1], strerror(errno));
		return (1);
	}
	newpwd = resolve_pwd(oldpwd, path);
	map_set(shell->envmap, "PWD", newpwd);
	free(newpwd);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 14:04:45 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/06 07:31:32 by hichikaw         ###   ########.fr       */
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

static char	*get_resolved_path(char *oldpwd, char *path)
{
	char	*resolved;

	if (path[0] == '/')
		return (ft_strdup(path));
	resolved = resolve_pwd(oldpwd, path);
	return (resolved);
}

static int	update_pwd(t_shell *shell, char *oldpwd, char *path)
{
	char	*newpwd;
	char	*cwd;

	newpwd = resolve_pwd(oldpwd, path);
	if (newpwd == NULL)
	{
		cwd = getcwd(NULL, 0);
		if (cwd != NULL)
		{
			map_set(shell->envmap, "PWD", cwd);
			free(cwd);
			return (0);
		}
		return (1);
	}
	map_set(shell->envmap, "PWD", newpwd);
	free(newpwd);
	return (0);
}

static char	*get_oldpwd(t_shell *shell)
{
	char	*oldpwd;
	char	*cwd_tmp;

	oldpwd = map_get(shell->envmap, "PWD");
	cwd_tmp = NULL;
	if (oldpwd == NULL)
	{
		cwd_tmp = getcwd(NULL, 0);
		oldpwd = cwd_tmp;
	}
	map_set(shell->envmap, "OLDPWD", oldpwd);
	if (cwd_tmp != NULL)
		free(cwd_tmp);
	return (map_get(shell->envmap, "OLDPWD"));
}

int	builtin_cd(t_shell *shell, char **argv)
{
	char	*oldpwd;
	char	path[PATH_MAX];
	char	*resolved;

	oldpwd = get_oldpwd(shell);
	if (get_target_path(shell, path, argv) != 0)
		return (1);
	resolved = get_resolved_path(oldpwd, path);
	if (resolved == NULL)
		return (1);
	if (chdir(resolved) < 0)
	{
		builtin_error("cd", argv[1], strerror(errno));
		free(resolved);
		return (1);
	}
	if (update_pwd(shell, oldpwd, path) != 0)
	{
		builtin_error("cd", NULL, "cannot determine directory");
		free(resolved);
		return (1);
	}
	free(resolved);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/07 18:54:56 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "minishell.h"

static char	*check_path_access(char *path)
{
	char	*dup;

	if (access(path, X_OK) == 0)
	{
		dup = ft_strdup(path);
		if (dup == NULL)
			fatal_error("ft_strdup");
		return (dup);
	}
	return (NULL);
}

static void	build_path(char *path, const char *dir, size_t len,
		const char *filename)
{
	ft_bzero(path, PATH_MAX);
	if (len > 0)
		ft_strlcpy(path, dir, len + 1);
	else
		ft_strlcpy(path, dir, PATH_MAX);
	ft_strlcat(path, "/", PATH_MAX);
	ft_strlcat(path, filename, PATH_MAX);
}

char	*search_path(t_shell *shell, const char *filename)
{
	char	path[PATH_MAX];
	char	*value;
	char	*end;
	char	*result;

	value = xgetenv(shell, "PATH");
	if (value == NULL)
		return (NULL);
	while (*value)
	{
		end = ft_strchr(value, ':');
		if (end)
			build_path(path, value, end - value, filename);
		else
			build_path(path, value, 0, filename);
		result = check_path_access(path);
		if (result)
			return (result);
		if (end == NULL)
			return (NULL);
		value = end + 1;
	}
	return (NULL);
}

int	get_exit_status(int wstatus)
{
	if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	else
		return (WEXITSTATUS(wstatus));
}

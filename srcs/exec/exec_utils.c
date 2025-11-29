/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/29 09:29:01 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "minishell.h"

#include <string.h>

static char	*check_path_access(char *path)
{
	char	*dup;

	if (access(path, X_OK) == 0)
	{
		dup = strdup(path);
		if (dup == NULL)
			fatal_error("strdup");
		return (dup);
	}
	return (NULL);
}

static void	build_path(char *path, const char *dir, size_t len,
		const char *filename)
{
	bzero(path, PATH_MAX);
	if (len > 0)
		strncpy(path, dir, len);
	else
		ft_strlcpy(path, dir, PATH_MAX);
	ft_strlcat(path, "/", PATH_MAX);
	ft_strlcat(path, filename, PATH_MAX);
}

char	*search_path(const char *filename)
{
	char	path[PATH_MAX];
	char	*value;
	char	*end;
	char	*result;

	value = xgetenv("PATH");
	if (value == NULL)
		return (NULL);
	while (*value)
	{
		end = strchr(value, ':');
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

void	validate_access(const char *path, const char *filename)
{
	if (path == NULL)
		err_exit(filename, "command not found", 127);
	if (access(path, F_OK) < 0)
		err_exit(filename, "command not found", 127);
}

int	get_exit_status(int wstatus)
{
	if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	else
		return (WEXITSTATUS(wstatus));
}

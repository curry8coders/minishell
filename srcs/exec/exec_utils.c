/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/03 15:53:01 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "minishell.h"

#include <string.h>

/**
 * Check whether a filesystem path is executable and return a heap-allocated copy when it is.
 *
 * @param path Path to test for execute permission.
 * @returns A malloc-allocated duplicate of `path` if the file is executable, `NULL` otherwise.
 *          The caller is responsible for freeing the returned string.
 *          On allocation failure, `fatal_error("strdup")` is invoked.
 */
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

/**
 * Construct a filesystem path by joining a directory and a filename into the provided buffer.
 *
 * The function writes the resulting string "dir/filename" into the `path` buffer,
 * clearing the buffer first. If `len` is greater than zero, only the first `len`
 * bytes of `dir` are used as the directory component; otherwise the full `dir`
 * string is used. If the combined result exceeds PATH_MAX it will be truncated.
 *
 * @param path Buffer where the combined path will be written (must have at least PATH_MAX bytes).
 * @param dir Directory component to use (may be partially used when `len` > 0).
 * @param len Number of bytes from `dir` to copy when > 0; if 0 the full `dir` is used.
 * @param filename Filename component to append after a '/' separator.
 */
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

/**
 * Locate an executable by searching each directory listed in the PATH environment variable.
 *
 * @param filename Name of the executable to locate; appended to each PATH directory to form candidate paths.
 * @returns A newly allocated string containing the full path to the first matching executable found; `NULL` if no executable is found or PATH is not set. The caller is responsible for freeing the returned string.
 */
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



/**
 * Map a process termination status from wait(2) to a conventional exit code.
 *
 * @param wstatus Status value returned by wait(2) or a related wait-family call.
 * @returns `128 + signal` if the process was terminated by a signal, otherwise the process's exit status.
 */
int	get_exit_status(int wstatus)
{
	if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	else
		return (WEXITSTATUS(wstatus));
}
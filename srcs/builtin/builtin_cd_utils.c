/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:44:02 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/30 20:58:44 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>
#include "minishell.h"

#include <string.h>

/**
 * Remove the final pathname component from a null-terminated POSIX-style path string.
 *
 * Modifies the string in-place by replacing the last '/' with a NUL terminator,
 * effectively truncating the path one component level. A path consisting only of
 * the root slash ("/") is left unchanged.
 *
 * @param path Mutable, null-terminated path buffer to be shortened (must not be NULL).
 */
void	delete_last_elm(char *path)
{
	char	*start;
	char	*last_slash_ptr;

	start = path;
	last_slash_ptr = NULL;
	while (*path)
	{
		if (*path == '/')
			last_slash_ptr = path;
		path++;
	}
	if (last_slash_ptr != start)
		*last_slash_ptr = '\0';
}

/**
 * Append the next path element from `src` onto `dst`, inserting a '/' separator if needed, and advance `rest` past the appended element.
 * @param dst Destination buffer containing the base path; must be null-terminated and large enough to receive an optional '/' and the next element from `src`.
 * @param rest Pointer to a `char*` which will be set to point into `src` immediately after the appended element (at a '/' or the terminating NUL).
 * @param src Source path string from which the next element (up to the next '/' or NUL) is taken.
 */
void	append_path_elm(char *dst, char **rest, char *src)
{
	size_t	elm_len;

	elm_len = 0;
	while (src[elm_len] && src[elm_len] != '/')
		elm_len++;
	if (dst[ft_strlen(dst) - 1] != '/')
		ft_strcat(dst, "/");
	ft_strncat(dst, src, elm_len);
	*rest = src + elm_len;
}

/**
 * Apply the next path component from *path to newpwd, updating both newpwd and *path.
 *
 * If the next component is a slash, it is skipped. If it is "." the component is skipped;
 * if it is ".." the last path element is removed from newpwd. Otherwise the next path
 * element is appended to newpwd. In all cases *path is advanced past the processed component.
 *
 * @param newpwd Buffer containing the path being constructed; modified in-place.
 * @param path Pointer to the remaining path string; *path is advanced past the consumed component.
 */
void	process_path_element(char *newpwd, char **path)
{
	if (**path == '/')
		(*path)++;
	else if (ft_strncmp(*path, ".", 1) == 0
		&& ((*path)[1] == '\0' || (*path)[1] == '/'))
		(*path)++;
	else if (ft_strncmp(*path, "..", 2) == 0
		&& ((*path)[2] == '\0' || (*path)[2] == '/'))
	{
		delete_last_elm(newpwd);
		(*path) += 2;
	}
	else
		append_path_elm(newpwd, path, *path);
}

/**
 * Resolve a new absolute working directory by applying `path` components to a base directory.
 *
 * @param oldpwd Base directory to start from when `path` is relative; if NULL, resolution starts from root.
 * @param path Path string whose components (/, ., .., and names) are applied in order; if `path` begins with '/', resolution starts from root.
 * @returns A newly allocated string containing the resolved absolute path. The caller is responsible for freeing the returned string.
 */
char	*resolve_pwd(char *oldpwd, char *path)
{
	char	newpwd[PATH_MAX];
	char	*dup;

	if (*path == '/' || oldpwd == NULL)
		ft_strlcpy(newpwd, "/", PATH_MAX);
	else
		ft_strlcpy(newpwd, oldpwd, PATH_MAX);
	while (*path)
		process_path_element(newpwd, &path);
	dup = ft_strdup(newpwd);
	if (dup == NULL)
		fatal_error("strdup");
	return (dup);
}
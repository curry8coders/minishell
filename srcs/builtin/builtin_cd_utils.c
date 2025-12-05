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

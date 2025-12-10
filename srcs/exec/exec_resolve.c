/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 15:35:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/10 10:52:33 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <sys/stat.h>
#include "libft.h"
#include "minishell.h"

static void	handle_resolve_error(char *path, char **argv,
	const char *errmsg, int exit_code)
{
	if (!(argv[0] == NULL))
		print_error(*argv, errmsg);
	if (path != argv[0])
		free(path);
	free_argv(argv);
	exit(exit_code);
}

static void	validate_path(char *path, char **argv, struct stat *st)
{
	if (stat(path, st) < 0)
		handle_resolve_error(path, argv, "command not found", 127);
	if (S_ISDIR(st->st_mode))
		handle_resolve_error(path, argv, "Is a directory", 126);
	if (access(path, X_OK) < 0)
		handle_resolve_error(path, argv, "Permission denied", 126);
}

char	*resolve_path(t_shell *shell, char **argv)
{
	char		*path;
	struct stat	st;

	path = argv[0];
	if (path == NULL)
	{
		free_argv(argv);
		exit(0);
	}
	if (path[0] == '\0')
		handle_resolve_error(path, argv, "command not found", 127);
	if (ft_strcmp(path, ".") == 0 || ft_strcmp(path, "..") == 0)
		handle_resolve_error(path, argv, "command not found", 127);
	if (ft_strchr(path, '/') == NULL)
	{
		path = search_path(shell, path);
		if (path == NULL)
			handle_resolve_error(path, argv, "command not found", 127);
	}
	validate_path(path, argv, &st);
	return (path);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 15:35:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/07 20:01:24 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// <<<<<<< HEAD
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include "minishell.h"

// static void	check_path_validity(char **argv, char *path)
// {
// 	if (path == NULL || path[0] == '\0')
// 	{
// 		command_not_found_error(argv[0]);
// 		free_argv(argv);
// 		exit(127);
// 	}
// 	if (ft_strcmp(path, ".") == 0 || ft_strcmp(path, "..") == 0)
// 	{
// 		command_not_found_error(argv[0]);
// 		free_argv(argv);
// 		exit(127);
// 	}
// }

// static char	*search_and_validate_path(t_shell *shell, char **argv, char *path)
// {
// 	if (ft_strchr(path, '/') == NULL)
// 	{
// 		path = search_path(shell, path);
// 		if (path == NULL)
// 		{
// 			command_not_found_error(argv[0]);
// 			free_argv(argv);
// 			exit(127);
// 		}
// 	}
// 	return (path);
// }

// static void	validate_path_stat(char **argv, char *path, struct stat *st)
// {
// 	if (stat(path, st) < 0)
// 	{
// 		command_not_found_error(argv[0]);
// 		if (path != NULL && path != argv[0])
// 			free(path);
// 		free_argv(argv);
// 		exit(127);
// 	}
// 	if (S_ISDIR(st->st_mode))
// 	{
// 		print_error(argv[0], "Is a directory");
// 		if (path != argv[0])
// 			free(path);
// 		free_argv(argv);
// 		exit(126);
// 	}
// }

// static void	validate_path_access(char **argv, char *path)
// {
// 	if (access(path, X_OK) < 0)
// 	{
// 		permission_denied_error(argv[0]);
// 		if (path != argv[0])
// 			free(path);
// 		free_argv(argv);
// 		exit(126);
// 	}
// }

// char	*resolve_path(t_shell *shell, char **argv)
// {
// 	char			*path;
// 	struct stat		st;

// 	path = argv[0];
// 	check_path_validity(argv, path);
// 	path = search_and_validate_path(shell, argv, path);
// 	validate_path_stat(argv, path, &st);
// 	validate_path_access(argv, path);
// =======
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include "minishell.h"

static void	handle_resolve_error(char *path, char **argv,
	const char *errmsg, int exit_code)
{
	print_error(*argv, errmsg);
	if (path != argv[0])
		free(path);
	free_argv(argv);
	exit(exit_code);
}

char	*resolve_path(t_shell *shell, char **argv)
{
	char		*path;
	struct stat	st;

	path = argv[0];
	if (path == NULL || path[0] == '\0'
		|| ft_strcmp(path, ".") == 0 || ft_strcmp(path, "..") == 0)
		handle_resolve_error(path, argv, "command not found", 127);
	if (ft_strchr(path, '/') == NULL)
	{
		path = search_path(shell, path);
		if (path == NULL)
			handle_resolve_error(path, argv, "command not found", 127);
	}
	if (stat(path, &st) < 0)
		handle_resolve_error(path, argv, "command not found", 127);
	if (S_ISDIR(st.st_mode))
		handle_resolve_error(path, argv, "Is a directory", 126);
	if (access(path, X_OK) < 0)
		handle_resolve_error(path, argv, "Permission denied", 126);
	return (path);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 15:35:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/06 16:47:55 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

// static void	handle_path_error(char **argv, char *path)
// {
// 	command_not_found_error(argv[0]);
// 	if (path != NULL && path != argv[0])
// 		free(path);
// 	free_argv(argv);
// 	exit(127);

// static char	*search_and_validate(t_shell *shell, char **argv, char *path)
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
// 	if (access(path, F_OK) < 0 || access(path, X_OK) < 0)
// 		handle_path_error(argv, path);
// 	return (path);
// }

// char	*resolve_path(t_shell *shell, char **argv)
// {
// 	char	*path;

// 	path = argv[0];
// 	if (path == NULL || path[0] == '\0')
// 	{
// 		command_not_found_error(argv[0]);
// 		free_argv(argv);
// 		exit(127);
// 	}
// 	return (search_and_validate(shell, argv, path));
// }

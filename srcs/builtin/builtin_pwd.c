/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:36:21 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/30 20:47:20 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include "minishell.h"

#include <stdio.h>
#include <string.h>

bool	equal_inode(const char *path1, const char *path2)
{
	struct stat	st1;
	struct stat	st2;

	memset(&st1, 0, sizeof(st1));
	memset(&st2, 0, sizeof(st2));
	if (stat(path1, &st1) < 0)
		fatal_error("stat");
	if (stat(path2, &st2) < 0)
		fatal_error("stat");
	return (st1.st_ino == st2.st_ino);
}

int	builtin_pwd(t_shell *shell, char **argv)
{
	char	*pwd;
	char	cwd[PATH_MAX];

	(void)argv;
	pwd = xgetenv(shell, "PWD");
	if (pwd == NULL || !equal_inode(pwd, "."))
	{
		if (getcwd(cwd, PATH_MAX) == NULL)
		{
			builtin_error("pwd", NULL, "getcwd");
			return (1);
		}
		write(STDOUT_FILENO, cwd, strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	else
	{
		write(STDOUT_FILENO, pwd, strlen(pwd));
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
}

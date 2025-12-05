/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:35:46 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/03 21:28:11 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

#define ERROR_PREFIX "minishell: "

bool	g_syntax_error = false;

void	perror_prefix(void)
{
	ft_dprintf(STDERR_FILENO, "%s", ERROR_PREFIX);
}

void	fatal_error(const char *msg)
{
	perror_prefix();
	ft_dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
	exit(1);
}

void	assert_error(const char *msg)
{
	perror_prefix();
	ft_dprintf(STDERR_FILENO, "Assert Error: %s\n", msg);
	exit(255);
}

void	print_error(const char *location, const char *msg)
{
	perror_prefix();
	ft_dprintf(STDERR_FILENO, "%s: %s\n", location, msg);
}

void	err_exit(const char *location, const char *msg, int status)
{
	print_error(location, msg);
	exit(status);
}

void	todo(const char *msg)
{
	perror_prefix();
	ft_dprintf(STDERR_FILENO, "TODO: %s\n", msg);
	exit(255);
}

void	command_not_found_error(const char *location)
{
	if (location == NULL | *location == '\0')
		location = "";
	print_error(location, "command not found");
}

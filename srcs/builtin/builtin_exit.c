/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 10:38:10 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/03 18:59:17 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "minishell.h"

#include <ctype.h>

int	g_exit_status;

bool	is_numeric(char *s)
{
	if (!isdigit(*s))
		return (false);
	while (*s)
	{
		if (!isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}

int	builtin_exit(char **argv)
{
	long	res;
	char	*arg;
	char	*endptr;

	if (argv[1] == NULL)
	{
		g_exit_status = g_last_status;
		return (BUILTIN_EXIT_REQUEST);
	}
	if (argv[2])
	{
		xperror("exit: too many arguments");
		return (1);
	}
	arg = argv[1];
	if (is_numeric(arg))
	{
		errno = 0;
		res = strtol(arg, &endptr, 10);
		if (errno == 0 && *endptr == '\0')
		{
			g_exit_status = (int)res;
			return (BUILTIN_EXIT_REQUEST);
		}
	}
	xperror("exit: numeric argument required");
	g_exit_status = 2;
	return (BUILTIN_EXIT_REQUEST);
}

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

/**
 * Check whether a null-terminated string consists of one or more ASCII digits.
 *
 * @param s Pointer to a null-terminated string to validate; must not be NULL.
 * @return `true` if `s` contains at least one character and every character is a digit, `false` otherwise.
 */
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

/**
 * Handle the shell builtin `exit`, validate its arguments, and signal a requested shell termination.
 *
 * If no argument is provided, sets `g_exit_status` to `g_last_status`. If a single numeric
 * argument is provided and successfully parsed, sets `g_exit_status` to that value. If the
 * single argument is not a valid number, sets `g_exit_status` to 2. When more than one
 * argument is supplied, reports an error and does not request termination nor change
 * `g_exit_status`.
 *
 * @param argv NULL-terminated argument vector where argv[0] is the command name and argv[1]
 *             (if present) is interpreted as the desired exit status.
 * @returns `BUILTIN_EXIT_REQUEST` to indicate the shell should terminate after `g_exit_status`
 *          has been set; returns `1` if too many arguments were provided and termination is not requested.
 */
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
		builtin_error("exit", NULL, "too many arguments");
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
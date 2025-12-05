/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:26:51 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/01 21:24:46 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

#include <string.h>

/**
 * Determine whether a string is an echo "-n" style option (a '-' followed only by one or more 'n' characters).
 *
 * @param arg Null-terminated string to test.
 * @returns `true` if `arg` starts with '-' and every following character is 'n', `false` otherwise.
 */
bool	check_only_n_options(char *arg)
{
	size_t	i;

	if (arg[0] != '-')
		return (false);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

/**
 * Print the provided arguments to standard output using echo semantics, accepting one or more `-n` options to suppress the trailing newline.
 *
 * @param argv Null-terminated argument vector where `argv[0]` is the command name; subsequent entries are printed separated by single spaces.
 * @returns 0 on success.
 */
int	builtin_echo(char **argv)
{
	bool	echo_newline;
	size_t	i;

	i = 1;
	echo_newline = true;
	while (argv[i] && check_only_n_options(argv[i]))
	{
		echo_newline = false;
		i++;
	}
	while (argv[i])
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		i++;
		if (argv[i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (echo_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
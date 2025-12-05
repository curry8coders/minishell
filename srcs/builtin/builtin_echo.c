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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:26:51 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/28 20:29:16 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

#include <string.h>

int	builtin_echo(char **argv)
{
	bool	echo_newline;
	size_t	i;

	i = 1;
	echo_newline = true;
	if (argv[1] && strncmp(argv[1], "-n", 2) == 0)
	{
		echo_newline = false;
		i++;
	}
	while (argv[i])
	{
		write(STDOUT_FILENO, argv[i], strlen(argv[i]));
		i++;
		if (argv[i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (echo_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 10:38:10 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/10 03:01:17 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

bool	is_numeric(char *s)
{
	if (s == NULL || *s == '\0')
		return (false);
	if (!ft_isdigit(*s))
		return (false);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}

static int	parse_exit_arg(t_shell *shell, char *arg)
{
	long	res;
	char	*endptr;

	if (is_numeric(arg))
	{
		errno = 0;
		res = ft_strtol(arg, &endptr, 10);
		if (errno == 0 && *endptr == '\0')
		{
			shell->exit_status = (int)res;
			return (BUILTIN_EXIT_REQUEST);
		}
	}
	builtin_error("exit", arg, "numeric argument required");
	shell->exit_status = 2;
	return (BUILTIN_EXIT_REQUEST);
}

int	builtin_exit(t_shell *shell, char **argv)
{
	if (argv[1] == NULL)
	{
		shell->exit_status = shell->last_status;
		return (BUILTIN_EXIT_REQUEST);
	}
	if (argv[2])
	{
		builtin_error("exit", NULL, "too many arguments");
		return (1);
	}
	return (parse_exit_arg(shell, argv[1]));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 14:30:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/06 15:04:55 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

void	todo(const char *msg)
{
	perror_prefix();
	ft_dprintf(STDERR_FILENO, "TODO: %s\n", msg);
	exit(255);
}

void	command_not_found_error(const char *location)
{
	if (location == NULL || *location == '\0')
		location = "";
	print_error(location, "command not found");
}

void	permission_denied_error(const char *location)
{
	print_error(location, "Permission denied");
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 14:30:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/07 18:41:44 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

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

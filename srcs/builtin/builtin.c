/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:15:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/22 05:15:24 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

bool	is_builtin(t_node *node)
{
	const char		*cmd_name;

	if (node == NULL || node->command == NULL || node->command->args == NULL || node->command->args->word == NULL)
		return (false);
	cmd_name = node->command->args->word;
	if (strcmp(cmd_name, "echo") == 0
		|| strcmp(cmd_name, "cd") == 0
		|| strcmp(cmd_name, "pwd") == 0
		|| strcmp(cmd_name, "export") == 0
		|| strcmp(cmd_name, "unset") == 0
		|| strcmp(cmd_name, "env") == 0
		|| strcmp(cmd_name, "exit") == 0)
	{
		return (true);
	}
	return (false);
}

int	exec_builtin(t_node *node)
{
	(void)node;
	return (0);
}

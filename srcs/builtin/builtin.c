/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:15:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/03 18:59:26 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

#include <string.h>

/**
 * Execute a recognized shell builtin from the provided node.
 *
 * Applies the node's I/O redirections, runs the matched builtin with argv
 * converted from the node's token list, and then restores redirections.
 * If the executed builtin requests process termination (status == BUILTIN_EXIT_REQUEST),
 * this function will exit the process with the global exit status.
 *
 * @param node Parsed command node containing the command, arguments, and redirects.
 * @return The exit status returned by the executed builtin; may be BUILTIN_EXIT_REQUEST.
 */
int	exec_builtin(t_node *node)
{
	int		status;
	char	**argv;

	do_redirect(node->command->redirects);
	argv = token_list_to_argv(node->command->args);
	if (ft_strcmp(argv[0], "exit") == 0)
		status = builtin_exit(argv);
	else if (ft_strcmp(argv[0], "export") == 0)
		status = builtin_export(argv);
	else if (ft_strcmp(argv[0], "unset") == 0)
		status = builtin_unset(argv);
	else if (ft_strcmp(argv[0], "env") == 0)
		status = builtin_env(argv);
	else if (ft_strcmp(argv[0], "cd") == 0)
		status = builtin_cd(argv);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		status = builtin_pwd(argv);
	else if (ft_strcmp(argv[0], "echo") == 0)
		status = builtin_echo(argv);
	else
		todo("exec_builtin");
	free_argv(argv);
	reset_redirect(node->command->redirects);
	if (status == BUILTIN_EXIT_REQUEST)
		exit(g_exit_status);
	return (status);
}

/**
 * Determine whether the command in the given parse node is a recognized shell builtin.
 *
 * @param node Parse tree node containing the command to check; may be NULL.
 * @returns `true` if the node contains a command name that matches a recognized builtin, `false` otherwise.
 *          If `node`, `node->command`, `node->command->args`, or `node->command->args->word` is NULL,
 *          the function returns `false`.
 */
bool	is_builtin(t_node *node)
{
	const char		*cmd_name;
	int				i;
	static char		*builtin_commands[] = {
		"exit",
		"export",
		"unset",
		"env",
		"cd",
		"pwd",
		"echo",
		NULL
	};

	if (node == NULL || node->command == NULL || node->command->args == NULL
		|| node->command->args->word == NULL)
		return (false);
	cmd_name = node->command->args->word;
	i = 0;
	while (builtin_commands[i])
	{
		if (ft_strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}
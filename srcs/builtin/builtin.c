/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:15:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/29 03:38:22 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <string.h>

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
	return (status);
}

const char	**set_builtin_commands(void)
{
	static const char	**builtin_commands;

	builtin_commands = (const char **)malloc(sizeof(char *) * 8);
	if (!builtin_commands)
		return (NULL);
	builtin_commands[0] = "exit";
	builtin_commands[1] = "export";
	builtin_commands[2] = "unset";
	builtin_commands[3] = "env";
	builtin_commands[4] = "cd";
	builtin_commands[5] = "pwd";
	builtin_commands[6] = "echo";
	builtin_commands[7] = NULL;
	return (builtin_commands);
}

bool	is_builtin(t_node *node)
{
	const char			*cmd_name;
	static const char	**builtin_commands;
	unsigned int		i;

	builtin_commands = set_builtin_commands();
	if (!builtin_commands)
		return (false);
	if (node == NULL || node->command == NULL || node->command->args == NULL
		|| node->command->args->word == NULL)
		return (false);
	cmd_name = node->command->args->word;
	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

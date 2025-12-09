/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:15:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/05 08:15:56 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "libft.h"
#include "minishell.h"

static int	dispatch_builtin(t_shell *shell, char **argv)
{
	if (ft_strcmp(argv[0], "exit") == 0)
		return (builtin_exit(shell, argv));
	else if (ft_strcmp(argv[0], "export") == 0)
		return (builtin_export(shell, argv));
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (builtin_unset(shell, argv));
	else if (ft_strcmp(argv[0], "env") == 0)
		return (builtin_env(shell, argv));
	else if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(shell, argv));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd(shell, argv));
	else if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argv));
	return (1);
}

int	exec_builtin(t_shell *shell, t_node *node)
{
	int		status;
	char	**argv;

	do_redirect(node->command->redirects);
	argv = token_list_to_argv(node->command->args);
	status = dispatch_builtin(shell, argv);
	free_argv(argv);
	reset_redirect(node->command->redirects);
	if (status == BUILTIN_EXIT_REQUEST)
	{
		write(STDERR_FILENO, "exit\n", 5);
		exit(shell->exit_status);
	}
	return (status);
}

bool	is_builtin(t_node *node)
{
	const char		*cmd_name;
	int				i;
	static char		*builtin_commands[] = {
		"exit", "export", "unset", "env", "cd", "pwd", "echo", NULL
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

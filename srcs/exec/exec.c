/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:55:05 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/05 08:15:56 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include "minishell.h"

static pid_t	exec_pipeline(t_shell *shell, t_node *node);
static int		wait_pipeline(pid_t last_pid);

int	exec(t_shell *shell, t_node *node)
{
	pid_t	last_pid;
	int		status;

	if (open_redir_file(shell, node) < 0)
	{
		close_all_redirect_fds(node);
		return (ERROR_OPEN_REDIR);
	}
	if (node->next == NULL && is_builtin(node))
		status = exec_builtin(shell, node);
	else
	{
		last_pid = exec_pipeline(shell, node);
		status = wait_pipeline(last_pid);
		close_all_redirect_fds(node);
	}
	return (status);
}

static char	*resolve_path(t_shell *shell, char **argv)
{
	char	*path;

	path = argv[0];
	if (strchr(path, '/') == NULL)
		path = search_path(shell, path);
	if (path == NULL || access(path, F_OK) < 0)
	{
		print_error(argv[0], "command not found");
		free_argv(argv);
		if (path != NULL && path != argv[0])
			free(path);
		exit(127);
	}
	return (path);
}

static int	exec_nonbuiltin(t_shell *shell, t_node *node)
{
	char	*path;
	char	**argv;
	char	**envp;

	do_redirect(node->command->redirects);
	argv = token_list_to_argv(node->command->args);
	path = resolve_path(shell, argv);
	envp = get_environ(shell->envmap);
	execve(path, argv, envp);
	free_argv(envp);
	free_argv(argv);
	if (path != argv[0])
		free(path);
	reset_redirect(node->command->redirects);
	fatal_error("execve");
}

static pid_t	exec_pipeline(t_shell *shell, t_node *node)
{
	pid_t	pid;

	if (node == NULL)
		return (-1);
	prepare_pipe(node);
	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	else if (pid == 0)
	{
		reset_signal();
		close_all_redirect_fds(node->next);
		prepare_pipe_child(node);
		if (is_builtin(node))
			exit(exec_builtin(shell, node));
		else
			exec_nonbuiltin(shell, node);
	}
	prepare_pipe_parent(node);
	if (node->command)
		close_redirect_fds(node->command->redirects);
	if (node->next)
		return (exec_pipeline(shell, node->next));
	return (pid);
}

static int	wait_pipeline(pid_t last_pid)
{
	pid_t	wait_result;
	int		status;
	int		wstatus;

	status = 0;
	while (1)
	{
		wait_result = wait(&wstatus);
		if (wait_result == last_pid)
			status = get_exit_status(wstatus);
		else if (wait_result < 0)
		{
			if (errno == ECHILD)
				break ;
			else if (errno == EINTR)
				continue ;
			else
				fatal_error("wait");
		}
	}
	return (status);
}

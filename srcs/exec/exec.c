/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:55:05 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/06 02:48:38 by ichikawahik      ###   ########.fr       */
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
		ignore_sig(SIGINT);
		last_pid = exec_pipeline(shell, node);
		status = wait_pipeline(last_pid);
		setup_sigint();
		close_all_redirect_fds(node);
	}
	return (status);
}

static int	exec_nonbuiltin(t_shell *shell, t_node *node)
{
	char	*path;
	char	**argv;
	char	**envp;

	do_redirect(node->command->redirects);
	argv = token_list_to_argv(node->command->args);
	path = resolve_path(shell, argv);
	if (path == NULL)
	{
		free_argv(argv);
		reset_redirect(node->command->redirects);
		exit(127);
	}
	envp = get_environ(shell->envmap);
	execve(path, argv, envp);
	fatal_error("execve");
}

static pid_t	fork_and_exec(t_shell *shell, t_node *node)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		xperror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		reset_signal();
		close_all_redirect_fds(node->next);
		prepare_pipe_child(node);
		if (is_builtin(node))
			exit(exec_builtin(shell, node));
		exec_nonbuiltin(shell, node);
	}
	return (pid);
}

static pid_t	exec_pipeline(t_shell *shell, t_node *node)
{
	pid_t	pid;

	if (node == NULL || prepare_pipe(node) < 0)
		return (-1);
	pid = fork_and_exec(shell, node);
	if (pid < 0)
		return (-1);
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
		handle_child_status(wait_result, last_pid, &status, wstatus);
		if (wait_result < 0)
		{
			if (errno == ECHILD)
				break ;
			else if (errno == EINTR)
				continue ;
			else
			{
				xperror("wait");
				break ;
			}
		}
	}
	return (status);
}

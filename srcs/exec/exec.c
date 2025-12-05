/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:55:05 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/03 21:13:47 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "minishell.h"
#include <stdio.h>

#include <string.h>

static pid_t	exec_pipeline(t_node *node);
static int		wait_pipeline(pid_t last_pid);

/**
 * Execute a command node or pipeline, handling redirections and builtin optimization.
 *
 * Executes the provided command node: opens required redirection files, runs a single builtin
 * directly when appropriate, or spawns a pipeline of processes and waits for completion.
 * All redirect file descriptors are closed before returning when applicable.
 *
 * @param node Head node of a command or pipeline to execute.
 * @returns The exit status of the executed command (for pipelines, the status of the last command).
 *          Returns ERROR_OPEN_REDIR if opening redirection files failed.
 */
int	exec(t_node *node)
{
	pid_t	last_pid;
	int		status;

	if (open_redir_file(node) < 0)
	{
		close_all_redirect_fds(node);
		return (ERROR_OPEN_REDIR);
	}
	if (node->next == NULL && is_builtin(node))
		status = exec_builtin(node);
	else
	{
		last_pid = exec_pipeline(node);
		status = wait_pipeline(last_pid);
		close_all_redirect_fds(node);
	}
	return (status);
}

/**
 * Execute a non-builtin command from the given AST node.
 *
 * Applies the node's redirections, builds argv from the node's tokens, resolves the executable
 * (searching PATH when the command name contains no '/'), verifies the file exists, and invokes
 * execve with the current environment. On missing or inaccessible commands the function prints
 * an error and exits with status 127; if execve returns the function reports a fatal error.
 *
 * @param node AST node containing the command and its redirects/arguments.
 */
int	exec_nonbuiltin(t_node *node)
{
	char	*path;
	char	**argv;
	char	**envp;

	do_redirect(node->command->redirects);
	argv = token_list_to_argv(node->command->args);
	path = argv[0];
	
	if (path == NULL || path[0] == '\0')
	{
		command_not_found_error(argv[0]);
		free_argv(argv);
		exit(127);
	}
	
	if (strchr(path, '/') == NULL)
	{
		path = search_path(path);
		if (path == NULL)
		{
			command_not_found_error(argv[0]);
			free_argv(argv);
			exit(127);
		}
	}
		
	if (access(path, F_OK) < 0)
	{
		command_not_found_error(argv[0]);
		if (path != argv[0])
			free(path);
		free_argv(argv);
		exit(127);
	}
	envp = get_environ(g_envmap);
	execve(path, argv, envp);
	free_argv(envp);
	free_argv(argv);
	if (path != argv[0])
		free(path);
	reset_redirect(node->command->redirects);
	fatal_error("execve");
}

/**
 * Execute a command pipeline starting at the given node, creating processes and wiring pipes between stages.
 *
 * Sets up necessary pipes, forks per pipeline stage, and in children performs redirection and command execution
 * (builtin or non-builtin). Parent continues building the pipeline and returns the PID of the last spawned child.
 *
 * @param node First node of the pipeline to execute; each node represents one pipeline stage.
 * @returns PID of the last child process in the pipeline, or -1 if `node` is NULL.
 */
static pid_t	exec_pipeline(t_node *node)
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
			exit(exec_builtin(node));
		else
			exec_nonbuiltin(node);
	}
	prepare_pipe_parent(node);
	if (node->command)
		close_redirect_fds(node->command->redirects);
	if (node->next)
		return (exec_pipeline(node->next));
	return (pid);
}

/**
 * Waits for all child processes and captures the exit status of the pipeline's last process.
 *
 * Blocks until there are no remaining child processes; when a waited PID equals `last_pid`,
 * the function records that child's exit status for return. Interrupted waits (EINTR) are
 * retried, and the loop ends when there are no children (ECHILD). Any other wait error
 * triggers a fatal error.
 *
 * @param last_pid PID of the last process in the pipeline whose exit status should be returned.
 * @returns The recorded exit status of `last_pid` if observed, or 0 if `last_pid` was not found.
 */
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
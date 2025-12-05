/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 05:58:26 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/03 21:13:47 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "minishell.h"

/**
 * Duplicate a file descriptor to the lowest available descriptor number >= 10 with the close-on-exec flag.
 *
 * @param fd Original file descriptor to duplicate.
 * @returns The duplicated file descriptor number (>= 10).
 *
 * On failure, calls fatal_error("fcntl").
 */
int	stashfd(int fd)
{
	int	stashfd;

	stashfd = fcntl(fd, F_DUPFD_CLOEXEC, 10);
	if (stashfd < 0)
		fatal_error("fcntl");
	return (stashfd);
}

/**
 * Determine whether a syntax tree node represents a redirection.
 *
 * @param node Pointer to the node to test; must not be NULL.
 * @returns `true` if the node represents an I/O redirection (ND_REDIR_OUT, ND_REDIR_IN, ND_REDIR_APPEND, or ND_REDIR_HEREDOC), `false` otherwise.
 */
bool	is_redirect(t_node *node)
{
	if (node->kind == ND_REDIR_OUT)
		return (true);
	else if (node->kind == ND_REDIR_IN)
		return (true);
	else if (node->kind == ND_REDIR_APPEND)
		return (true);
	else if (node->kind == ND_REDIR_HEREDOC)
		return (true);
	return (false);
}

/**
 * Apply a redirect node and its successors, replacing their target file descriptors with the redirect file descriptors.
 *
 * For each redirect node, the original target fd is saved in `stashed_targetfd`, the redirect's `filefd` is duplicated
 * onto `targetfd`, and the original `filefd` is closed and set to -1. If `redir` is NULL the function returns immediately.
 * If a node is not a redirect, `assert_error("do_redirect")` is invoked.
 *
 * @param redir Pointer to the head of a linked list of redirect nodes to apply.
 */
void	do_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	if (is_redirect(redir))
	{
		redir->stashed_targetfd = stashfd(redir->targetfd);
		dup2(redir->filefd, redir->targetfd);
		if (redir->filefd >= 0)
		{
			close(redir->filefd);
			redir->filefd = -1;
		}
	}
	else
		assert_error("do_redirect");
	do_redirect(redir->next);
}

/**
 * Close and invalidate the file descriptor for a redirect node and all following nodes.
 *
 * For each node in the linked list starting at `redir` that represents a redirect,
 * closes `filefd` if it is >= 0 and sets it to -1. The function processes nodes
 * recursively via the `next` pointer.
 *
 * @param redir Pointer to the first redirect node to process; may be NULL.
 */
void	close_redirect_fds(t_node *redir)
{
	if (redir == NULL)
		return ;
	if (is_redirect(redir))
	{
		if (redir->filefd >= 0)
		{
			close(redir->filefd);
			redir->filefd = -1;
		}
	}
	close_redirect_fds(redir->next);
}

/**
 * Recursively close all open redirect file descriptors in a node tree.
 *
 * Traverses the given AST node and its children; for pipeline nodes it
 * processes both the pipeline command and the next pipeline element, and for
 * simple command nodes it closes any open file descriptor on their redirect
 * list. Passing NULL is a no-op.
 *
 * @param node Root AST node whose redirects should be closed (may be NULL).
 */
void	close_all_redirect_fds(t_node *node)
{
	if (node == NULL)
		return ;
	if (node->kind == ND_PIPELINE)
	{
		close_all_redirect_fds(node->command);
		close_all_redirect_fds(node->next);
	}
	else if (node->kind == ND_SIMPLE_CMD)
		close_redirect_fds(node->redirects);
}

/**
 * Close redirect file descriptors for every pipeline node in a list except the specified current node.
 *
 * Traverses pipeline nodes beginning at `head` and closes each node's redirect `filefd` via
 * close_redirect_fds unless the node pointer equals `current`. If `current` is NULL, all pipeline
 * nodes' redirect file descriptors will be closed.
 *
 * @param head Head of the pipeline node list to traverse.
 * @param current Pipeline node to exclude from closing redirects (may be NULL to exclude none).
 */
void	close_pipeline_fds_except_current(t_node *head, t_node *current)
{
	if (head == NULL)
		return ;
	if (head->kind == ND_PIPELINE)
	{
		if (head != current && head->command)
			close_redirect_fds(head->command->redirects);
		close_pipeline_fds_except_current(head->next, current);
	}
}

/**
 * Restore and clean up a linked list of redirect nodes starting from the tail.
 *
 * For each redirect node (processed from tail to head) this function closes any
 * open redirect `filefd`, restores the original `targetfd` from
 * `stashed_targetfd` using `dup2`, closes the stashed descriptor, and marks
 * both descriptors as closed by setting them to -1. Non-redirect nodes cause
 * an assertion failure.
 *
 * @param redir Head of a linked list of redirect nodes; may be NULL.
 */
void	reset_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	reset_redirect(redir->next);
	if (is_redirect(redir))
	{
		if (redir->filefd >= 0)
		{
			close(redir->filefd);
			redir->filefd = -1;
		}
		if (redir->stashed_targetfd >= 0)
		{
			dup2(redir->stashed_targetfd, redir->targetfd);
			close(redir->stashed_targetfd);
			redir->stashed_targetfd = -1;
		}
	}
	else
		assert_error("reset_redirect");
}
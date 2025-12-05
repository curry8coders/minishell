/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 07:09:28 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/03 17:59:58 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

static	void	cpy_pipe(int dst[2], int src[2]);

void	prepare_pipe(t_node *node)
{
	if (node->next == NULL)
		return ;
	if (pipe(node->outpipe) < 0)
		fatal_error("pipe");
	cpy_pipe(node->next->inpipe, node->outpipe);
}

/**
 * Configure standard input and output for a child process using the node's pipes.
 *
 * Duplicates node->inpipe[0] onto STDIN_FILENO if inpipe[0] is a valid descriptor
 * and not already STDIN, and duplicates node->outpipe[1] onto STDOUT_FILENO if
 * outpipe[1] is a valid descriptor and not already STDOUT. Closes the corresponding
 * original descriptors and closes node->outpipe[0] if it is a valid descriptor.
 *
 * @param node Pointer to the pipeline node whose inpipe and outpipe file descriptors
 *             are used to set up the child's standard streams.
 */
void	prepare_pipe_child(t_node *node)
{
	if (node->outpipe[0] >= 0)
		close(node->outpipe[0]);
	if (node->inpipe[0] >= 0 && node->inpipe[0] != STDIN_FILENO)
	{
		dup2(node->inpipe[0], STDIN_FILENO);
		close(node->inpipe[0]);
	}
	if (node->outpipe[1] >= 0 && node->outpipe[1] != STDOUT_FILENO)
	{
		dup2(node->outpipe[1], STDOUT_FILENO);
		close(node->outpipe[1]);
	}
}

/**
 * Close pipe file descriptors for a node in the parent process after forking.
 *
 * Closes the node's read-end input descriptor if it is a valid descriptor and not STDIN_FILENO.
 * If the node has a successor, closes the node's write-end output descriptor when it is valid.
 *
 * @param node The pipeline node whose pipe descriptors should be closed in the parent.
 */
void	prepare_pipe_parent(t_node *node)
{
	if (node->inpipe[0] >= 0 && node->inpipe[0] != STDIN_FILENO)
		close(node->inpipe[0]);
	if (node->next && node->outpipe[1] >= 0)
		close(node->outpipe[1]);
}

static	void	cpy_pipe(int dst[2], int src[2])
{
	dst[0] = src[0];
	dst[1] = src[1];
}
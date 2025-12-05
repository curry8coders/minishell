/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 07:09:28 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/05 22:03:46 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

static	void	cpy_pipe(int dst[2], int src[2]);

int	prepare_pipe(t_node *node)
{
	if (node->next == NULL)
		return (0);
	if (pipe(node->outpipe) < 0)
	{
		xperror("pipe");
		return (-1);
	}
	cpy_pipe(node->next->inpipe, node->outpipe);
	return (0);
}

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

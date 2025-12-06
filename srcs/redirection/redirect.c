/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 05:58:26 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/06 15:42:49 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "minishell.h"

int	stashfd(int fd)
{
	int	new_fd;

	new_fd = dup(fd);
	if (new_fd < 0)
	{
		xperror("dup");
		return (-1);
	}
	if (new_fd < 10)
	{
		if (dup2(new_fd, 10) < 0)
		{
			xperror("dup2");
			close(new_fd);
			return (-1);
		}
		close(new_fd);
		return (10);
	}
	return (new_fd);
}

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

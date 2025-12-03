/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 05:58:26 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/03 20:22:38 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "minishell.h"

int	stashfd(int fd)
{
	int	stashfd;

	stashfd = fcntl(fd, F_DUPFD_CLOEXEC, 10);
	if (stashfd < 0)
		fatal_error("fcntl");
	return (stashfd);
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

// Reset must be done from tail to head
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

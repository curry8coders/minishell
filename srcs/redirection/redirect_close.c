/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_close.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:30:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/04 22:32:20 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

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

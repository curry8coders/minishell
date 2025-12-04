/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_open.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/30 20:03:16 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "minishell.h"

static int	open_redirect_file_node(t_shell *shell, t_node *node)
{
	if (node->kind == ND_REDIR_OUT)
		node->filefd = open(node->filename->word,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (node->kind == ND_REDIR_IN)
		node->filefd = open(node->filename->word, O_RDONLY);
	else if (node->kind == ND_REDIR_APPEND)
		node->filefd = open(node->filename->word,
				O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (node->kind == ND_REDIR_HEREDOC)
		node->filefd = read_heredoc(shell, node->delimiter->word,
				node->is_delim_unquoted);
	else
		assert_error("open_redirect_file_node");
	if (node->filefd < 0)
	{
		if (node->kind == ND_REDIR_OUT || node->kind == ND_REDIR_APPEND
			|| node->kind == ND_REDIR_IN)
			xperror(node->filename->word);
		return (-1);
	}
	return (0);
}

int	open_redir_file(t_shell *shell, t_node *node)
{
	if (node == NULL)
		return (0);
	if (node->kind == ND_PIPELINE)
	{
		if (open_redir_file(shell, node->command) < 0)
			return (-1);
		if (open_redir_file(shell, node->next) < 0)
			return (-1);
		return (0);
	}
	else if (node->kind == ND_SIMPLE_CMD)
		return (open_redir_file(shell, node->redirects));
	if (open_redirect_file_node(shell, node) < 0)
		return (-1);
	return (open_redir_file(shell, node->next));
}

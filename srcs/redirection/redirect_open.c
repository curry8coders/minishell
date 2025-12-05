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

/**
 * Open the file or heredoc associated with a redirection node and store its file descriptor in the node.
 *
 * Opens the target file for output (create/truncate), input (read), or append as appropriate for the node's redirection kind,
 * or obtains a heredoc file descriptor for heredoc redirections. On success, stores the resulting descriptor in node->filefd.
 *
 * @param node Redirection node whose target should be opened; its `filefd` field is updated on success.
 * @returns `0` on success, `-1` on failure (in which case an error is reported for file-based redirections).
 */
static int	open_redirect_file_node(t_node *node)
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
		node->filefd = read_heredoc(node->delimiter->word,
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

/**
 * Open all files required by the redirections in a syntax-tree node subtree.
 *
 * Recursively traverses pipeline, simple command, and redirection nodes and opens each redirection target
 * (including heredocs), storing the resulting file descriptor in the corresponding node.
 *
 * @param node Root of the command/redirect subtree to process; may be NULL.
 * @returns `0` on success, `-1` if any redirection file or heredoc fails to open.
 */
int	open_redir_file(t_node *node)
{
	if (node == NULL)
		return (0);
	if (node->kind == ND_PIPELINE)
	{
		if (open_redir_file(node->command) < 0)
			return (-1);
		if (open_redir_file(node->next) < 0)
			return (-1);
		return (0);
	}
	else if (node->kind == ND_SIMPLE_CMD)
		return (open_redir_file(node->redirects));
	if (open_redirect_file_node(node) < 0)
		return (-1);
	return (open_redir_file(node->next));
}
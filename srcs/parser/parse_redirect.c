/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/30 19:40:45 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "minishell.h"

/**
 * Create an AST node representing output redirection (">").
 * @param rest Pointer to the token pointer that will be advanced to the token following the filename.
 * @param tok The redirection token; its next token is used as the filename.
 * @returns Pointer to the newly allocated redirect node with `filename` set and `targetfd` set to `STDOUT_FILENO`.
 */
t_node	*redirect_out(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_OUT);
	node->filename = tokdup(tok->next);
	node->targetfd = STDOUT_FILENO;
	*rest = tok->next->next;
	return (node);
}

/**
 * Create an AST node representing input redirection.
 *
 * The created node's filename is set to a duplicate of the token following `tok`,
 * its target file descriptor is set to `STDIN_FILENO`, and `*rest` is advanced
 * to the token after the filename.
 *
 * @param rest Pointer to the current token pointer; advanced to the token after the filename.
 * @param tok  The redirection token whose following token contains the filename.
 * @returns Pointer to the newly allocated `ND_REDIR_IN` node.
 */
t_node	*redirect_input(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_IN);
	node->filename = tokdup(tok->next);
	node->targetfd = STDIN_FILENO;
	*rest = tok->next->next;
	return (node);
}

/**
 * Create an AST node representing an append redirection (`>>`) using the filename token following `tok`.
 * @param rest Pointer updated to the token after the filename.
 * @param tok Token for the redirection operator.
 * @returns Pointer to a newly allocated `ND_REDIR_APPEND` node with its `filename` set from the next token and `targetfd` set to `STDOUT_FILENO`.
 */
t_node	*redirect_append(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_APPEND);
	node->filename = tokdup(tok->next);
	node->targetfd = STDOUT_FILENO;
	*rest = tok->next->next;
	return (node);
}

/**
 * Create a heredoc redirection AST node from the given token and advance the token stream.
 *
 * The created node contains a duplicated delimiter token, marks the delimiter as
 * unquoted when it contains neither single nor double quote characters, and sets
 * the redirection target file descriptor to standard input.
 *
 * @param rest Pointer to the current token pointer; on return it is advanced to the token after the delimiter.
 * @param tok  Token representing the heredoc operator (whose next token is the delimiter).
 * @returns Pointer to a newly allocated `t_node` of type `ND_REDIR_HEREDOC`.
 */
t_node	*redirect_heredoc(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_HEREDOC);
	node->delimiter = tokdup(tok->next);
	if (strchr(node->delimiter->word, SINGLE_QUOTE_CHAR) == NULL
		&& strchr(node->delimiter->word, DOUBLE_QUOTE_CHAR) == NULL)
		node->is_delim_unquoted = true;
	node->targetfd = STDIN_FILENO;
	*rest = tok->next->next;
	return (node);
}
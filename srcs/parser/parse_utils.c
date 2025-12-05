/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/03 17:59:58 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "minishell.h"

/**
 * Check whether a token represents end-of-file.
 *
 * @param tok Token to inspect.
 * @returns `true` if the token's kind is `TK_EOF`, `false` otherwise.
 */
bool	at_eof(t_token *tok)
{
	return (tok->kind == TK_EOF);
}

/**
 * Check whether a token represents the specified operator.
 *
 * @param tok Token to check.
 * @param op Operator string to compare against.
 * @returns `true` if `tok` is an operator token and its word equals `op`, `false` otherwise.
 */
bool	equal_op(t_token *tok, char *op)
{
	if (tok->kind != TK_OP)
		return (false);
	return (strcmp(tok->word, op) == 0);
}

/**
 * Allocate and initialize a new parse node of the given kind.
 *
 * Initializes file descriptors and pipe endpoints to -1. On allocation failure
 * the function calls fatal_error("calloc").
 *
 * @param kind Kind of node to create.
 * @returns Pointer to the newly allocated and initialized t_node.
 */
t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = calloc(1, sizeof(*node));
	if (node == NULL)
		fatal_error("calloc");
	node->kind = kind;
	node->filefd = -1;
	node->stashed_targetfd = -1;
	node->inpipe[0] = -1;
	node->inpipe[1] = -1;
	node->outpipe[0] = -1;
	node->outpipe[1] = -1;
	return (node);
}

/**
 * Create a duplicate of a token with a newly allocated copy of its word.
 *
 * @param tok Token to duplicate; must be non-NULL.
 * @returns A newly allocated `t_token` whose `word` is a heap-allocated copy of `tok->word`
 *          and whose `kind` equals `tok->kind`. Ownership of the returned token (and its
 *          word) is transferred to the caller.
 */
t_token	*tokdup(t_token *tok)
{
	char	*word;

	word = strdup(tok->word);
	if (word == NULL)
		fatal_error("strdup");
	return (new_token(word, tok->kind));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 20:51:03 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/30 20:32:07 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

#include <stdio.h>

/**
 * Free all memory associated with a command node and its linked components.
 *
 * Frees the node's token lists (arguments, filename, delimiter), all child nodes
 * (redirects, next, command) and the node itself. Does nothing if `node` is NULL.
 *
 * @param node Pointer to the head of the node subtree to free.
 */
void	free_node(t_node *node)
{
	if (node == NULL)
		return ;
	free_tok(node->args);
	free_tok(node->filename);
	free_tok(node->delimiter);
	free_node(node->redirects);
	free_node(node->next);
	free_node(node->command);
	free(node);
}

/**
 * Release a token list and any associated word buffers.
 *
 * Recursively frees each token in the list: frees the token's `word` buffer if present, then frees the remainder of the list and the token structure itself.
 *
 * @param tok Pointer to the head of the token list; if NULL nothing is done.
 */
void	free_tok(t_token *tok)
{
	if (tok == NULL)
		return ;
	if (tok->word)
		free(tok->word);
	free_tok(tok->next);
	free(tok);
}

/**
 * Free a NULL-terminated array of heap-allocated strings and the array container.
 *
 * Frees each element in argv and then frees argv itself. If argv is NULL, no action is taken.
 *
 * @param argv NULL-terminated array of pointers to heap-allocated strings to free.
 */
void	free_argv(char **argv)
{
	int	i;

	if (argv == NULL)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
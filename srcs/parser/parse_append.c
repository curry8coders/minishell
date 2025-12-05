/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/30 19:46:14 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

/**
 * Append a token element to the end of a token singly linked list.
 *
 * If the list is empty, sets the head to the provided element; otherwise
 * traverses to the list's end and links the element as the new last node.
 *
 * @param tok Pointer to the head pointer of the token list to modify.
 * @param elm Token element to append (may be NULL; no action is taken if NULL).
 */
void	append_tok(t_token **tok, t_token *elm)
{
	if (*tok == NULL)
	{
		*tok = elm;
		return ;
	}
	append_tok(&(*tok)->next, elm);
}

/**
 * Append a node to the end of a singly linked list.
 *
 * Inserts `elm` as the last element of the list pointed to by `*node`.
 * If the list is empty (`*node` is NULL), `elm` becomes the new head.
 *
 * @param node Pointer to the head pointer of the node list to modify.
 * @param elm  Node to append to the list.
 */
void	append_node(t_node **node, t_node *elm)
{
	if (*node == NULL)
	{
		*node = elm;
		return ;
	}
	append_node(&(*node)->next, elm);
}
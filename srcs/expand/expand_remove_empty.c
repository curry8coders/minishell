/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_remove_empty.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 09:45:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/10 10:29:23 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

static bool	word_had_quotes(const char *word)
{
	const char	*p;

	p = word;
	while (*p)
	{
		if (*p == SINGLE_QUOTE_CHAR || *p == DOUBLE_QUOTE_CHAR
			|| *p == LITERAL_SINGLE_QUOTE || *p == LITERAL_DOUBLE_QUOTE)
			return (true);
		p++;
	}
	return (false);
}

static void	remove_empty_tokens_recursive(t_token **head_ref)
{
	t_token	*current;
	t_token	*next;

	if (*head_ref == NULL)
		return ;
	current = *head_ref;
	if (current->word && current->word[0] == '\0'
		&& !word_had_quotes(current->word))
	{
		next = current->next;
		free(current->word);
		free(current);
		*head_ref = next;
		remove_empty_tokens_recursive(head_ref);
		return ;
	}
	remove_empty_tokens_recursive(&current->next);
}

void	remove_empty_tokens(t_token **tok)
{
	if (tok == NULL)
		return ;
	remove_empty_tokens_recursive(tok);
}

void	remove_empty_tokens_node(t_node *node)
{
	if (node == NULL)
		return ;
	remove_empty_tokens(&node->args);
	remove_empty_tokens_node(node->redirects);
	remove_empty_tokens_node(node->command);
	remove_empty_tokens_node(node->next);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:54:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/10 10:52:33 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

void	expand_quote_removal(t_node *node)
{
	if (node == NULL)
		return ;
	remove_quote(node->args);
	remove_quote(node->filename);
	remove_quote(node->delimiter);
	restore_literal_quotes_tok(node->args);
	restore_literal_quotes_tok(node->filename);
	restore_literal_quotes_tok(node->delimiter);
	expand_quote_removal(node->redirects);
	expand_quote_removal(node->command);
	expand_quote_removal(node->next);
}

void	expand(t_shell *shell, t_node *node)
{
	expand_variable_node(shell, node);
	remove_empty_tokens_node(node);
	expand_quote_removal(node);
}

char	*expand_heredoc_line(t_shell *shell, char *line)
{
	t_strbuf	buf;
	char		*p;

	p = line;
	strbuf_init(&buf);
	while (*p)
	{
		if (is_variable(p))
			expand_variable(shell, &buf, &p, false);
		else if (is_special_parameter(p))
			expand_special_param(shell, &buf, &p, p);
		else
			strbuf_append_char(&buf, *p++);
	}
	free(line);
	return (strbuf_finish(&buf));
}

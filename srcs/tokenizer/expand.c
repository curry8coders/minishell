/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:54:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/04 22:38:25 by ichikawahik      ###   ########.fr       */
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
	expand_quote_removal(node->redirects);
	expand_quote_removal(node->command);
	expand_quote_removal(node->next);
}

void	expand(t_node *node)
{
	expand_variable_node(node);
	expand_quote_removal(node);
}

char	*expand_heredoc_line(char *line)
{
	t_strbuf	buf;
	char		*p;

	p = line;
	strbuf_init(&buf);
	while (*p)
	{
		if (is_variable(p))
			expand_variable(&buf, &p, p);
		else if (is_special_parameter(p))
			expand_special_parameter(&buf, &p, p);
		else
			strbuf_append_char(&buf, *p++);
	}
	free(line);
	return (strbuf_finish(&buf));
}

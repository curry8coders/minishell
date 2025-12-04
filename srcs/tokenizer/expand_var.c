/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/04 22:00:00 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

void	append_num(t_strbuf *buf, unsigned int num)
{
	if (num == 0)
	{
		strbuf_append_char(buf, '0');
		return ;
	}
	if (num / 10 != 0)
		append_num(buf, num / 10);
	strbuf_append_char(buf, '0' + (num % 10));
}

void	expand_special_param(t_shell *sh, t_strbuf *buf, char **rest, char *p)
{
	if (!is_special_parameter(p))
		assert_error("Expected special parameter");
	p += 2;
	append_num(buf, sh->last_status);
	*rest = p;
}

void	expand_variable(t_shell *shell, t_strbuf *buf, char **rest, char *p)
{
	t_strbuf	name_buf;
	char		*value;

	strbuf_init(&name_buf);
	if (*p != '$')
		assert_error("Expected dollar sign");
	p++;
	if (!is_alpha_under(*p))
		assert_error("Variable must start with alpha or underscore.");
	strbuf_append_char(&name_buf, *p++);
	while (is_alpha_num_under(*p))
		strbuf_append_char(&name_buf, *p++);
	value = xgetenv(shell, name_buf.data);
	free(name_buf.data);
	if (value)
		strbuf_append_str(buf, value);
	*rest = p;
}

void	expand_variable_tok(t_shell *shell, t_token *tok)
{
	t_strbuf	buf;
	char		*p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	strbuf_init(&buf);
	while (*p && !is_metacharacter(*p))
	{
		if (*p == SINGLE_QUOTE_CHAR)
			append_single_quote(&buf, &p, p);
		else if (*p == DOUBLE_QUOTE_CHAR)
			append_double_quote(shell, &buf, &p, p);
		else if (is_variable(p))
			expand_variable(shell, &buf, &p, p);
		else if (is_special_parameter(p))
			expand_special_param(shell, &buf, &p, p);
		else
			strbuf_append_char(&buf, *p++);
	}
	free(tok->word);
	tok->word = strbuf_finish(&buf);
	expand_variable_tok(shell, tok->next);
}

void	expand_variable_node(t_shell *shell, t_node *node)
{
	if (node == NULL)
		return ;
	expand_variable_tok(shell, node->args);
	expand_variable_tok(shell, node->filename);
	expand_variable_node(shell, node->redirects);
	expand_variable_node(shell, node->command);
	expand_variable_node(shell, node->next);
}

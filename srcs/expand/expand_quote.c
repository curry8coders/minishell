/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/05 08:15:56 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

void	remove_single_quote(t_strbuf *buf, char **rest, char *p)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		p++;
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed single quote in remove_single_quote");
			strbuf_append_char(buf, *p++);
		}
		p++;
		*rest = p;
	}
	else
		assert_error("Expected single quote");
}

void	remove_double_quote(t_strbuf *buf, char **rest, char *p)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		p++;
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed double quote in remove_double_quote");
			strbuf_append_char(buf, *p++);
		}
		p++;
		*rest = p;
	}
	else
		assert_error("Expected double quote");
}

void	remove_quote(t_token *tok)
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
			remove_single_quote(&buf, &p, p);
		else if (*p == DOUBLE_QUOTE_CHAR)
			remove_double_quote(&buf, &p, p);
		else
			strbuf_append_char(&buf, *p++);
	}
	free(tok->word);
	tok->word = strbuf_finish(&buf);
	remove_quote(tok->next);
}

void	append_single_quote(t_strbuf *buf, char **rest, char *p)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		strbuf_append_char(buf, *p++);
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed single quote in append_single_quote");
			strbuf_append_char(buf, *p++);
		}
		strbuf_append_char(buf, *p++);
		*rest = p;
	}
	else
		assert_error("Expected single quote");
}

void	append_double_quote(t_shell *sh, t_strbuf *buf, char **rest, char *p)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		strbuf_append_char(buf, *p++);
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed double quote in append_double_quote");
			else if (is_variable(p))
				expand_variable(sh, buf, &p, true);
			else if (is_special_parameter(p))
				expand_special_param(sh, buf, &p, p);
			else
				strbuf_append_char(buf, *p++);
		}
		strbuf_append_char(buf, *p++);
		*rest = p;
	}
	else
		assert_error("Expected double quote");
}

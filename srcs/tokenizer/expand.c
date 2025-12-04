/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:54:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/04 20:35:46 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include <string.h>
#include <ctype.h>

#define STRBUF_INIT_CAP 16

typedef struct s_strbuf
{
	char	*data;
	size_t	len;
	size_t	capacity;
}	t_strbuf;

static void	strbuf_init(t_strbuf *buf)
{
	buf->capacity = STRBUF_INIT_CAP;
	buf->data = malloc(buf->capacity);
	if (buf->data == NULL)
		fatal_error("malloc");
	buf->data[0] = '\0';
	buf->len = 0;
}

static void	strbuf_grow(t_strbuf *buf, size_t needed)
{
	char	*new_data;
	size_t	new_cap;

	if (buf->len + needed < buf->capacity)
		return ;
	new_cap = buf->capacity * 2;
	while (buf->len + needed >= new_cap)
		new_cap *= 2;
	new_data = malloc(new_cap);
	if (new_data == NULL)
		fatal_error("malloc");
	ft_memcpy(new_data, buf->data, buf->len + 1);
	free(buf->data);
	buf->data = new_data;
	buf->capacity = new_cap;
}

static void	strbuf_append_char(t_strbuf *buf, char c)
{
	strbuf_grow(buf, 2);
	buf->data[buf->len++] = c;
	buf->data[buf->len] = '\0';
}

static void	strbuf_append_str(t_strbuf *buf, const char *s)
{
	size_t	slen;

	if (s == NULL)
		return ;
	slen = ft_strlen(s);
	strbuf_grow(buf, slen + 1);
	ft_memcpy(buf->data + buf->len, s, slen + 1);
	buf->len += slen;
}

static char	*strbuf_finish(t_strbuf *buf)
{
	return (buf->data);
}

static void	remove_single_quote(t_strbuf *buf, char **rest, char *p)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		p++;
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
			{
				tokenize_error("remove_single_quote", rest, p);
				return ;
			}
			strbuf_append_char(buf, *p++);
		}
		p++;
		*rest = p;
	}
	else
		assert_error("Expected single quote");
}

static void	remove_double_quote(t_strbuf *buf, char **rest, char *p)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		p++;
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
			{
				tokenize_error("remove_double_quote", rest, p);
				return ;
			}
			strbuf_append_char(buf, *p++);
		}
		p++;
		*rest = p;
	}
	else
		assert_error("Expected double quote");
}

static void	remove_quote(t_token *tok)
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

static void	append_num(t_strbuf *buf, unsigned int num)
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

static void	expand_special_parameter(t_strbuf *buf, char **rest, char *p)
{
	if (!is_special_parameter(p))
		assert_error("Expected special parameter");
	p += 2;
	append_num(buf, g_last_status);
	*rest = p;
}

static void	expand_variable(t_strbuf *buf, char **rest, char *p)
{
	t_strbuf	name_buf;
	char		*value;

	strbuf_init(&name_buf);
	if (*p != '$')
		assert_error("Expected dollar sign");
	p++;
	if (!is_alpha_under(*p))
		assert_error(\
			"Variable must starts with alphabetic character or underscore.");
	strbuf_append_char(&name_buf, *p++);
	while (is_alpha_num_under(*p))
		strbuf_append_char(&name_buf, *p++);
	value = xgetenv(name_buf.data);
	free(name_buf.data);
	if (value)
		strbuf_append_str(buf, value);
	*rest = p;
}

static void	append_single_quote(t_strbuf *buf, char **rest, char *p)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		strbuf_append_char(buf, *p++);
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
			{
				tokenize_error("append_single_quote", rest, p);
				return ;
			}
			strbuf_append_char(buf, *p++);
		}
		strbuf_append_char(buf, *p++);
		*rest = p;
	}
	else
		assert_error("Expected single quote");
}

static void	append_double_quote(t_strbuf *buf, char **rest, char *p)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		strbuf_append_char(buf, *p++);
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
			{
				tokenize_error("append_double_quote", rest, p);
				return ;
			}
			else if (is_variable(p))
				expand_variable(buf, &p, p);
			else if (is_special_parameter(p))
				expand_special_parameter(buf, &p, p);
			else
				strbuf_append_char(buf, *p++);
		}
		strbuf_append_char(buf, *p++);
		*rest = p;
	}
	else
		assert_error("Expected double quote");
}

static void	expand_variable_tok(t_token *tok)
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
			append_double_quote(&buf, &p, p);
		else if (is_variable(p))
			expand_variable(&buf, &p, p);
		else if (is_special_parameter(p))
			expand_special_parameter(&buf, &p, p);
		else
			strbuf_append_char(&buf, *p++);
	}
	free(tok->word);
	tok->word = strbuf_finish(&buf);
	expand_variable_tok(tok->next);
}

static void	expand_variable_node(t_node *node)
{
	if (node == NULL)
		return ;
	expand_variable_tok(node->args);
	expand_variable_tok(node->filename);
	expand_variable_node(node->redirects);
	expand_variable_node(node->command);
	expand_variable_node(node->next);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:54:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/14 22:54:57 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

#include <string.h>

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(size);
	if (new == NULL)
		fatal_error("malloc");
	if (*s)
	{
		strncpy(new, *s, size - 1);
		new[size - 1] = '\0';
	}
	else
		new[0] = '\0';
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}
//Critical performance issue:
// O(n²) complexity for string building.
// この実装だと1文字追加するたびに、毎回新しくメモリ確保をしていることになる
// バッファの概念を導入するとさらに良い rabbitくんより

void	remove_single_quote(char **dst, char **rest, char *p)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		// skip quote
		p++;
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed single quote");
			append_char(dst, *p++);
		}
		// skip quote
		p++;
		*rest = p;
	}
	else
		assert_error("Expected single quote");
}

void	remove_double_quote(char **dst, char **rest, char *p)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		// skip quote
		p++;
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed double quote");
			append_char(dst, *p++);
		}
		// skip quote
		p++;
		*rest = p;
	}
	else
		assert_error("Expected double quote");
}

void	remove_quote(t_token *tok)
{
	char	*new_word;
	char	*p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = NULL;
	while (*p && !is_metacharacter(*p))
	{
		if (*p == SINGLE_QUOTE_CHAR)
			remove_single_quote(&new_word, &p, p);
		else if (*p == DOUBLE_QUOTE_CHAR)
			remove_double_quote(&new_word, &p, p);
		else
			append_char(&new_word, *p++);
	}
	free(tok->word);
	tok->word = new_word;
	remove_quote(tok->next);
}
void	expand_quote_removal(t_node *node)
{
	if (node == NULL)
		return ;
	remove_quote(node->args);
	remove_quote(node->filename);
	expand_quote_removal(node->redirects);
	expand_quote_removal(node->next);
}

void	expand(t_node *node)
{
	expand_quote_removal(node);
}

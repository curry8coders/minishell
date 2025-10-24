/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:54:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/10/24 20:10:18 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenize.h"

void	append_char(char **s, char c)
{
	size_t size;
	char *new;

	size = 2;
	if (*s)
		size = strlen(*s);
	new = malloc(size);
	if (new == NULL)
		fatal_error("malloc");
	if (*s)
		strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

void	quote_removal(t_token *tok)
{
	char *new_word;
	char *p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = NULL;
	while (*p == SIGNAL_QUOTE_CHAR)
	{
		if (*p == SIGNAL_QUOTE_CHAR)
		{
		p++;
		while (*p != SIGNAL_QUOTE_CHAR)
		{
			if (*p == '\0')
				todo("Unclosed single quote");
			append_char(&new_word, *p++);
		}
		p++;
		}
		else
			append_char(&new_word, *p++);
	}
	free(tok->word);
	tok->word = new_word;
	quote_removal(tok->next);
}

void expand(t_token *tok)
{
	quote_removal(tok);
}
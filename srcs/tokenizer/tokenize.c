/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:57:17 by hichikaw          #+#    #+#             */
/*   Updated: 2025/10/25 17:29:41 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include <stdlib.h>>
#include "../../includes/minishell.h"

t_token *new_token(char *word, t_token_kind kind)
{
	t_token *tok;

	tok = calloc(1, sizeof(*tok));
	if (tok == NULL)
		fatal_error("calloc");
	tok->word = word;
	tok->kind = kind;
	return (tok);
}

bool is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool consume_blank(char **rest, char *line)
{
	if (is_blank(*line))
	{
		while (*line && is_blank(*line))
			line++;
		return (true);
	}
	*rest = line;
	return (false);
}

bool startswith(const char *s, const char *keyword)
{
	return (memcmp(s, keyword, strlen(keyword)) == 0);
}

bool is_operator(const char *s)
{
	static char *const operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
	size_t i = 0;

	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(s, operators[i]))
			return (true);
		i++;
	}
	return (false);
}

bool is_metacharacter(char c)
{
	return (c && strchr("|&;()\t\n", c));
}

bool is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}

t_token *operator(char **rest, char *line)
{
	static char *const operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
	size_t i = 0;
	char *op;
	
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(line, operators[i]))
		{
			op = strdup(operators[i]);
			if (op == NULL)
				fatal_error("strdup");
			*rest = line + strlen(op);
			return (new_token(op, TK_OP));
		}
		i++;
	}
	assert_error("Unexpected operator");
}

t_token *word(char **rest, char *line)
{
	const char *start = line;
	char *word;

	while (*line && !is_metacharacter(*line))
		line++;
	{
		if (*line == SINGLE_QUOTE_CHAR)
		{
			line++;
			while (*line && *line != SINGLE_QUOTE_CHAR)
			{
				if (*line == '\0')
					todo("Unclosed single quote");
				line++;
			}
			line++;
		}
		else
			line++;
	}
	word = strndup(start, line - start);
	if (word == NULL)
		fatal_error("strndup");
	*rest = line;
	return (new_token(word, TK_WORD));
}

bool	syntax_error = false;

void	takenize_error(const char *location, char **rest, char *line)
{
	syntax_error = true;
	dprint(STDERR_FILENO, "minishell: syntax error near %s\n", location);
	while (*line)
		line++;
	*rest = line;
}

t_token *tokenize(char *line)
{
	t_token head;
	t_token *tok;

	head.next = NULL;
	tok = &head;
	while (*line)
	{
		if (consume_blank(&line, line))
			continue;
		else if (is_operator(line))
			tok->next = operator(&line, line);
		else if (is_word(line))
			tok->next = word(&line, line);
		else
			assert_error("Unexpected character");
	}
	tok = tok->next;
	return (head.next);
}

void	interpret(char *line, int *stat_loc)
{
	t_token	*tok;
	char	*argv;
	tok = tokenize(line);
	if (tok->kind == TK_EOF)
		;
	else if (syntax_error)
		*stat_loc = ERROR_TOKENIZE;
	else
	{
		expand(tok);
		argv = token_list_to_argv(tok);
		*stat_loc = exec(argv);
		free_argv(argv);
	}
	free_tok(tok);
}

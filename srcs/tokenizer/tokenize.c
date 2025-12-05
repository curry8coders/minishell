/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:57:17 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/03 21:28:33 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include <stdlib.h>
#include "minishell.h"

extern bool g_syntax_error;

/**
 * Allocate and initialize a new token with the given word and kind.
 *
 * @param word The string to use as the token's word; ownership of this pointer
 *             is transferred to the returned token.
 * @param kind The token kind to assign.
 * @returns Pointer to the newly allocated t_token.
 *
 * On allocation failure, calls fatal_error("calloc").
 */
t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*tok;

	tok = calloc(1, sizeof(*tok));
	if (tok == NULL)
		fatal_error("calloc");
	tok->word = word;
	tok->kind = kind;
	return (tok);
}

/**
 * Selects the longest operator that matches the start of `line` and returns it as a token.
 *
 * @param rest On success, set to point immediately after the matched operator within `line`.
 * @param line The input string to scan for a leading operator.
 * @returns A newly allocated `TK_OP` token whose `word` is a heap-allocated copy of the matched operator.
 *          On allocation failure `fatal_error("strdup")` is called. If no operator matches, `assert_error("Unexpected operator")` is invoked.
 */
t_token	*operator(char **rest, char *line)
{
	static const char *const	operators[] = {">>", "<<", "||", "&&", ";;",
		"<", ">", "&", ";", "(", ")", "|", "\n"};
	size_t				i;
	char				*op;

	i = 0;
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

/**
 * Convert an input command line into a linked list of lexical tokens.
 *
 * Tokenization produces a singly linked list of t_token nodes representing
 * words, operators, and a terminating TK_EOF token. Tokenization skips
 * whitespace and stops early if a syntax error occurs; the global flag
 * `g_syntax_error` is cleared at the start and may be set to true if an error
 * is encountered.
 *
 * @param line Null-terminated input string to tokenize.
 * @returns Pointer to the first token in the resulting linked list (the list
 *          is terminated by a token of kind `TK_EOF`).
t_token	*tokenize(char *line)
{
	t_token	head;
	t_token	*tok;

	g_syntax_error = false;
	head.next = NULL;
	tok = &head;
	while (*line && g_syntax_error == false)
	{
		if (consume_blank(&line, line))
			continue ;
		else if (is_metacharacter(*line))
		{
			tok->next = operator(&line, line);
			tok = tok->next;
		}
		else if (is_word(line))
		{
			tok->next = word(&line, line);
			tok = tok->next;
		}
		else
			assert_error("Unexpected Token");
	}
	tok->next = new_token(NULL, TK_EOF);
	return (head.next);
}

char	**tail_recursive(t_token *tok, int nargs, char **argv)
{
	char	**new_argv;

	if (tok == NULL || tok->kind == TK_EOF)
		return (argv);
	new_argv = realloc(argv, (nargs + 2) * sizeof(char *));
	if (new_argv == NULL)
	{
		free(argv);
		fatal_error("realloc");
	}
	argv = new_argv;
	argv[nargs] = strdup(tok->word);
	if (argv[nargs] == NULL)
		fatal_error("strdup");
	argv[nargs + 1] = NULL;
	return (tail_recursive(tok->next, nargs + 1, argv));
}

/**
 * Convert a linked list of tokens into a NULL-terminated argv-style array of strings.
 *
 * @param tok Head of the token list to convert; traversal continues until a TK_EOF or NULL is reached.
 * @returns A heap-allocated NULL-terminated array of C strings containing the tokens' words.
 *          The caller owns the returned array and must free each string and the array itself.
 */
char	**token_list_to_argv(t_token *tok)
{
	char	**argv;

	argv = calloc(1, sizeof(char *));
	if (argv == NULL)
		fatal_error("calloc");
	return (tail_recursive(tok, 0, argv));
}

/*
man bash
DEFINITIONS
       The following definitions are used throughout the rest of this document.
       blank  A space or tab.
       word   A sequence of characters considered as a single unit by the shell.  Also known as a token.
       name   A word consisting only of alphanumeric characters and underscores, and beginning with an alphabetic
              character or an underscore.  Also referred to as an identifier.
       metacharacter
              A character that, when unquoted, separates words.  One of the following:
              |  & ; ( ) < > space tab
       control operator
              A token that performs a control function.  It is one of the following symbols:
              || & && ; ;; ( ) | <newline>
*/
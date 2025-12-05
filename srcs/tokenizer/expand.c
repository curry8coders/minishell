/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:54:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/30 20:23:52 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include <string.h>
#include <ctype.h>

/**
 * Append a character to a dynamically allocated null-terminated string, reallocating and updating the pointer.
 *
 * Allocates a new buffer sized to hold the existing string (if any), the appended character, and a terminating NUL,
 * copies the original contents if present, places `c` at the end, null-terminates, frees the old buffer, and updates
 * `*s` to point at the new buffer.
 *
 * @param s Pointer to the string pointer to modify; may point to NULL to create a new single-character string.
 * @param c Character to append to the string.
 *
 * On allocation failure, calls fatal_error("malloc").
 */
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
		ft_strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

/**
 * Copy characters between matching single quotes into the destination string and advance the input pointer past the closing quote.
 *
 * Processes an input location `p` that must point to a single quote, appends each character found between the opening and closing single quotes to the dynamically managed string referenced by `*dst`, and updates `*rest` to point immediately after the closing single quote.
 *
 * If the end of the input (`'\0'`) is encountered before a matching closing single quote, this function reports a tokenization error via `tokenize_error` and returns early without advancing past a closing quote. If `p` does not point to a single quote on entry, the function triggers `assert_error("Expected single quote")`.
 *
 * @param dst Address of the destination string pointer; characters between the quotes are appended to the string `*dst`.
 * @param rest Pointer to the input pointer that will be updated to the position after the closing single quote.
 * @param p Current input pointer; must point to the opening single quote on entry.
 */
void	remove_single_quote(char **dst, char **rest, char *p)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		p++;
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
			{
				tokenize_error("remove_single_quote", rest, p);//beta
				return ;
			}
			append_char(dst, *p++);
		}
		p++;
		*rest = p;
	}
	else
		assert_error("Expected single quote");
}

/**
 * Remove surrounding double quotes from the input at p and append the quoted content to dst.
 *
 * Advances past the opening double quote at p, copies characters up to (but not including)
 * the closing double quote into *dst, and sets *rest to the position after the closing quote.
 * If the string ends before a closing quote is found, reports a tokenize error and returns
 * without updating *rest. If p does not point to a double-quote, triggers an assertion error.
 *
 * @param dst Pointer to the destination string buffer pointer; characters are appended to the string it points to.
 * @param rest Pointer updated to point just after the closing double quote on success.
 * @param p    Input pointer expected to point at the opening double-quote character.
 */
void	remove_double_quote(char **dst, char **rest, char *p)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		p++;
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
			{
				tokenize_error("remove_double_quote", rest, p);//beta
				return ;
			}
			append_char(dst, *p++);
		}
		p++;
		*rest = p;
	}
	else
		assert_error("Expected double quote");
}

/**
 * Remove surrounding quotes from a token's word and update tokens recursively.
 *
 * For a token of kind TK_WORD with a non-NULL word, builds a new string by
 * copying characters from the original word until a metacharacter is reached.
 * Single-quoted and double-quoted segments are copied without their enclosing
 * quote characters (the quoted contents are preserved). The original word is
 * freed and replaced with the new, quote-stripped string. If the input token
 * is NULL, not a TK_WORD, or has a NULL word, the function returns immediately.
 *
 * The function processes the current token only up to the first metacharacter
 * and then recurses to the next token in the list.
 *
 * @param tok Token to process; may be NULL.
 *
 * Note: The function will abort via fatal_error("calloc") if initial allocation
 * of the new word buffer fails.
 */
void	remove_quote(t_token *tok)
{
	char	*new_word;
	char	*p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = calloc(1, sizeof(char));
	if (new_word == NULL)
		fatal_error("calloc");
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

/**
 * Recursively remove quoting characters from word tokens within a node tree.
 *
 * Processes the node in-place: strips quotes from its args, filename, and delimiter
 * token lists and then recurses into redirects, command, and next nodes.
 *
 * @param node Root of the node subtree to process; if NULL the function does nothing.
 */
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

/**
 * Append the decimal representation of an unsigned integer to a dynamic string.
 *
 * Appends the digits of `num` (base 10) to the string pointed to by `*dst`, growing
 * the buffer as needed. If `*dst` is NULL it will be allocated. If `num` is 0,
 * the character `'0'` is appended.
 *
 * @param dst Pointer to the dynamic string buffer; updated to point to the (possibly reallocated) buffer containing the appended digits.
 * @param num Unsigned integer whose decimal digits will be appended to `*dst`.
 */
void	append_num(char **dst, unsigned int num)
{
	if (num == 0)
	{
		append_char(dst, '0');
		return ;
	}
	if (num / 10 != 0)
		append_num(dst, num / 10);
	append_char(dst, '0' + (num % 10));
}

/**
 * Expand a special parameter at the current input position by appending the
 * global last status value to the destination string.
 *
 * This function expects `p` to point at a recognized special parameter
 * sequence (e.g., `$?`). It advances `p` past the parameter, appends the
 * decimal representation of `g_last_status` to `*dst`, and stores the
 * updated input position in `*rest`.
 *
 * @param dst Pointer to the destination string pointer to append into; the
 *            function updates the string content (and allocation) via this pointer.
 * @param rest Output pointer that will be set to the input position after the
 *             consumed special parameter.
 * @param p Current input position, must point to a special parameter sequence.
 *
 * @note The function triggers an assertion failure if `p` does not point to a
 *       special parameter.
 */
void	expand_special_parameter_str(char **dst, char **rest, char *p)
{
	if (!is_special_parameter(p))
		assert_error("Expected special parameter");
	p += 2;
	append_num(dst, g_last_status);
	*rest = p;
}

/**
 * Expand a shell-style variable reference at the current input position and append its value to the destination string.
 *
 * Parses a variable name beginning at `p` (which must start with '$' followed by an alphabetic character or underscore),
 * looks up its value with `xgetenv`, appends the value (if any) to `*dst`, and sets `*rest` to the position after the variable name.
 *
 * @param dst Pointer to the destination string pointer; the variable's value will be appended to the string referenced by `*dst`.
 * @param rest Output pointer that will be updated to point to the character following the parsed variable name in `p`.
 * @param p Input position; must point to a '$' that introduces the variable reference.
 *
 * Notes:
 * - On allocation failure this function calls `fatal_error("calloc")`.
 * - If `p` does not start with '$' or the character after '$' is not an alphabetic character or underscore, this function calls `assert_error`.
 */
void	expand_variable_str(char **dst, char **rest, char *p)
{
	char	*name;
	char	*value;

	name = calloc(1, sizeof(char));
	if (name == NULL)
		fatal_error("calloc");
	if (*p != '$')
		assert_error("Expected dollar sign");
	p++;
	if (!is_alpha_under(*p))
		assert_error(\
			"Variable must starts with alphabetic character or underscore.");
	append_char(&name, *p++);
	while (is_alpha_num_under(*p))
		append_char(&name, *p++);
	value = xgetenv(name);
	free(name);
	if (value)
		while (*value)
			append_char(dst, *value++);
	*rest = p;
}

/**
 * Append a single-quoted sequence (including surrounding single quotes) from `p` into `*dst`.
 *
 * If `*p` is a single-quote character, appends the opening quote, then appends all characters
 * up to the matching closing single quote, appends the closing quote, and updates `*rest`
 * to point just past the closing quote. If the input terminates before a closing quote is found,
 * calls `tokenize_error("append_single_quote", rest, p)` and returns without updating `*rest`.
 * If `*p` is not a single quote, calls `assert_error("Expected single quote")`.
 *
 * @param dst Pointer to the destination string pointer that will receive appended characters.
 * @param rest Pointer to a char* that will be set to the position after the closing quote on success.
 * @param p   Input pointer expected to point at the opening single quote.
 */
void	append_single_quote(char **dst, char **rest, char *p)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		append_char(dst, *p++);
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
			{
				tokenize_error("append_single_quote", rest, p);//beta
				return ;
			}
			append_char(dst, *p++);
		}
		append_char(dst, *p++);
		*rest = p;
	}
	else
		assert_error("Expected single quote");
}

/**
 * Process a double-quoted segment starting at p, appending its processed contents to dst and advancing rest past the closing quote.
 *
 * Appends the opening and closing double quotes and expands variables and special parameters encountered inside the quotes. If p does not point to a double quote, triggers assert_error. If a closing double quote is not found before the end of the string, calls tokenize_error and returns without advancing rest.
 *
 * @param dst Pointer to a dynamically allocated string buffer; characters are appended and the buffer may be reallocated.
 * @param rest Out-parameter updated to point immediately after the closing double quote on success.
 * @param p Pointer into the input string that must point to the opening double quote.
 */
void	append_double_quote(char **dst, char **rest, char *p)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		// skip quote
		append_char(dst, *p++);
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
			{
				tokenize_error("append_double_quote", rest, p);//beta
				return ;
			}
			else if (is_variable(p))
				expand_variable_str(dst, &p, p);
			else if (is_special_parameter(p))
				expand_special_parameter_str(dst, &p, p);
			else
				append_char(dst, *p++);
		}
		// skip quote
		append_char(dst, *p++);
		*rest = p;
	}
	else
		assert_error("Expected double quote");
}

void	expand_variable_tok(t_token *tok)
{
	char *new_word;
	char *p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = calloc(1, sizeof(char));
	if (new_word == NULL)
		fatal_error("calloc");
	while (*p && !is_metacharacter(*p))
	{
		if (*p == SINGLE_QUOTE_CHAR)
			append_single_quote(&new_word, &p, p);
		else if (*p == DOUBLE_QUOTE_CHAR)
			append_double_quote(&new_word, &p, p);
		else if (is_variable(p))
			expand_variable_str(&new_word, &p, p);
		else if (is_special_parameter(p))
			expand_special_parameter_str(&new_word, &p, p);
		else
			append_char(&new_word, *p++);
	}
	free(tok->word);
	tok->word = new_word;
	expand_variable_tok(tok->next);
}

void	expand_variable(t_node *node)
{
	if (node == NULL)
		return ;
	expand_variable_tok(node->args);
	expand_variable_tok(node->filename);
	// do not expand heredoc delimiter
	expand_variable(node->redirects);
	expand_variable(node->command);
	expand_variable(node->next);
}

void	expand(t_node *node)
{
	expand_variable(node);
	expand_quote_removal(node);
}

/**
 * Expand variables and special parameters in a heredoc line.
 *
 * Processes the entire input string, replacing variable references and special
 * parameter sequences with their expanded values and building a new string
 * containing the results.
 * @param line Heap-allocated NUL-terminated string containing the heredoc line;
 *             this function frees `line` before returning.
 * @returns A newly allocated NUL-terminated string containing the expanded line.
 *          On allocation failure, `fatal_error("calloc")` is called.
 */
char	*expand_heredoc_line(char *line)
{
	char	*new_word;
	char	*p;

	p = line;
	new_word = calloc(1, sizeof(char));
	if (new_word == NULL)
		fatal_error("calloc");
	while (*p)
	{
		if (is_variable(p))
			expand_variable_str(&new_word, &p, p);
		else if (is_special_parameter(p))
			expand_special_parameter_str(&new_word, &p, p);
		else
			append_char(&new_word, *p++);
	}
	free(line);
	return (new_word);
}

// void	append_char(char **s, char c)
//Critical performance issue:
// O(n²) complexity for string building.
// この実装だと1文字追加するたびに、毎回新しくメモリ確保をしていることになる
// バッファの概念を導入するとさらに良い rabbitくんより
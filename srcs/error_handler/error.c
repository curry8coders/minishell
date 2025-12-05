/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:35:46 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/03 21:28:11 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

#define ERROR_PREFIX "minishell: "

bool	g_syntax_error = false;

/**
 * Print the standard error prefix to standard error.
 *
 * This helper writes the configured ERROR_PREFIX string to STDERR_FILENO.
 */
static	void	perror_prefix(void)
{
	dprintf(STDERR_FILENO, "%s", ERROR_PREFIX);
}

void	fatal_error(const char *msg)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
	exit(1);
}

/**
 * Print an assertion error message to standard error and terminate the process.
 *
 * @param msg Human-readable message describing the assertion failure.
 *
 * @note This function does not return; it exits the program with status code 255.
 */
void	assert_error(const char *msg)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "Assert Error: %s\n", msg);
	exit(255);
}

/ **
 * Print a formatted error message with the program's standard error prefix to stderr.
 *
 * @param location Short identifier of where the error occurred (e.g., command or component).
 * @param msg Human-readable error description to display after the location.
 */
void	print_error(const char *location, const char *msg)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "%s: %s\n", location, msg);
}

/**
 * Print a formatted error message for the given location and message, then terminate the process with the specified status.
 *
 * @param location Context string shown before the error message (e.g., command or component name).
 * @param msg Error message to display.
 * @param status Exit status code used when terminating the process.
 */
void	err_exit(const char *location, const char *msg, int status)
{
	print_error(location, msg);
	exit(status);
}

void	todo(const char *msg)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "TODO: %s\n", msg);
	exit(255);
}

/**
 * Report a tokenization syntax error and advance the input pointer to the end of the current line.
 *
 * Prints a formatted syntax error message to standard error (either "unexpected end of input" if
 * the current character is NUL, or "near unexpected character '<char>'" otherwise), sets the
 * global syntax error flag `g_syntax_error` to true, and updates `*rest` to point to the end of
 * `line`.
 *
 * @param location Context or location label to include in the printed error message.
 * @param rest Pointer to a char* that will be updated to point to the end of the provided line.
 * @param line Current input position; the function inspects the character at this pointer to
 *             determine the reported error and then advances to the string terminator.
 */
void	tokenize_error(const char *location, char **rest, char *line)
{
	g_syntax_error = true;
	perror_prefix();
	if (*line == '\0')
		dprintf(STDERR_FILENO, "syntax error: unexpected end of input in %s\n", location);
	else
		dprintf(STDERR_FILENO, "syntax error near unexpected character '%c' in %s\n", *line, location);
	while (*line)
		line++;
	*rest = line;
}

/**
 * Report a syntax error for an unexpected token and advance the token iterator to the end.
 *
 * Sets the global syntax error flag, prints a standardized syntax error message to standard
 * error that includes the unexpected token's word and the provided location, and advances
 * the provided token pointer to the EOF token (or end of the token list) storing the result
 * into `*rest`.
 *
 * @param location Human-readable context or component name where the error occurred.
 * @param rest Output pointer that will be updated to point to the EOF token (or NULL) after traversal.
 * @param tok The token at which the unexpected condition was detected; its `word` is used in the printed message.
 */
void	parse_error(const char *location, t_token **rest, t_token *tok)
{
	g_syntax_error = true;
	perror_prefix();
	dprintf(STDERR_FILENO, "syntax error near unexpected token `%s' in %s\n", tok->word, location);
	//note : 
	// tokenize() 関数で作られる TK_EOF トークンは、wordに
	// NULLを設定した状態で生成される そこでparse_error()に
	// EOFトークンが渡されてtok->wordにアクセスするとセグフォー
	while(tok && !at_eof(tok))
		tok = tok->next;
	*rest = tok;
}

void	xperror(const char *location)
{
	perror_prefix();
	perror(location);
}

/**
 * Print a formatted builtin error message to standard error with the standard prefix.
 *
 * @param func Name of the builtin or subsystem producing the error.
 * @param name Optional subject of the error (e.g., an argument or identifier); may be NULL.
 * @param err Human-readable error message to display.
 */
void	builtin_error(const char *func, const char *name, const char *err)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "%s: ", func);
	if (name)
		dprintf(STDERR_FILENO, "%s: ", name);
	dprintf(STDERR_FILENO, "%s\n", err);
}

/**
 * Report a "command not found" error for the given location.
 *
 * @param location Context or command name to include in the error message. 
 */
void	command_not_found_error(const char *location)
{
	print_error(location, "command not found");
}
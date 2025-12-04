/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/04 21:00:00 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

void	tokenize_error(const char *location, char **rest, char *line)
{
	g_syntax_error = true;
	perror_prefix();
	if (*line == '\0')
		dprintf(STDERR_FILENO, "syntax error: unexpected end of input\n");
	else
		dprintf(STDERR_FILENO, "syntax error: unexpected character '%c'\n",
			*line);
	while (*line)
		line++;
	*rest = line;
	(void)location;
}

void	parse_error(t_token **rest, t_token *tok)
{
	g_syntax_error = true;
	perror_prefix();
	dprintf(STDERR_FILENO, "syntax error near unexpected token `%s'\n",
		tok->word);
	while (tok && !at_eof(tok))
		tok = tok->next;
	*rest = tok;
}

void	xperror(const char *location)
{
	perror_prefix();
	perror(location);
}

void	builtin_error(const char *func, const char *name, const char *err)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "%s: ", func);
	if (name)
		dprintf(STDERR_FILENO, "%s: ", name);
	dprintf(STDERR_FILENO, "%s\n", err);
}

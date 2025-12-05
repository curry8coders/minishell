/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/06 04:07:51 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

void	tokenize_error(t_shell *sh, const char *loc, char **rest, char *line)
{
	sh->syntax_error = true;
	perror_prefix();
	if (*line == '\0')
		write(STDERR_FILENO, "syntax error: unexpected end of input\n", 38);
	else
	{
		write(STDERR_FILENO, "syntax error: unexpected character '", 36);
		write(STDERR_FILENO, line, 1);
		write(STDERR_FILENO, "'\n", 2);
	}
	while (*line)
		line++;
	*rest = line;
	(void)loc;
}

void	parse_error(t_shell *shell, t_token **rest, t_token *tok)
{
	shell->syntax_error = true;
	perror_prefix();
	write(STDERR_FILENO, "syntax error near unexpected token `", 36);
	write(STDERR_FILENO, tok->word, ft_strlen(tok->word));
	write(STDERR_FILENO, "'\n", 2);
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
	write(STDERR_FILENO, func, ft_strlen(func));
	write(STDERR_FILENO, ": ", 2);
	if (name)
	{
		write(STDERR_FILENO, name, ft_strlen(name));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, err, ft_strlen(err));
	write(STDERR_FILENO, "\n", 1);
}

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

#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "minishell.h"

void	tokenize_error(t_shell *sh, const char *loc, char **rest, char *line)
{
	sh->syntax_error = true;
	perror_prefix();
	if (*line == '\0')
		ft_dprintf(STDERR_FILENO, "syntax error: unexpected end of input\n");
	else
		ft_dprintf(STDERR_FILENO, "syntax error: unexpected character '%c'\n",
			*line);
	while (*line)
		line++;
	*rest = line;
	(void)loc;
}

void	parse_error(t_shell *shell, t_token **rest, t_token *tok)
{
	shell->syntax_error = true;
	perror_prefix();
	ft_dprintf(STDERR_FILENO, "syntax error near unexpected token `%s'\n",
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
	ft_dprintf(STDERR_FILENO, "%s: ", func);
	if (name)
		ft_dprintf(STDERR_FILENO, "%s: ", name);
	ft_dprintf(STDERR_FILENO, "%s\n", err);
}

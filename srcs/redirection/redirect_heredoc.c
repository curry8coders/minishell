/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/30 20:03:15 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include "minishell.h"

#include <string.h>

bool	g_readline_interrupted = false;

static bool	process_heredoc_line(int pfd, const char *delimiter,
	bool is_delim_unquoted)
{
	char	*line;

	line = readline("> ");
	if (line == NULL)
		return (false);
	if (g_readline_interrupted)
	{
		free(line);
		return (false);
	}
	if (strcmp(line, delimiter) == 0)
	{
		free(line);
		return (false);
	}
	if (is_delim_unquoted)
		line = expand_heredoc_line(line);
	dprintf(pfd, "%s\n", line);
	free(line);
	return (true);
}

int	read_heredoc(const char *delimiter, bool is_delim_unquoted)
{
	int		pfd[2];

	if (pipe(pfd) < 0)
		fatal_error("pipe");
	g_readline_interrupted = false;
	while (process_heredoc_line(pfd[1], delimiter, is_delim_unquoted))
		;
	close(pfd[1]);
	if (g_readline_interrupted)
	{
		close(pfd[0]);
		return (-1);
	}
	return (pfd[0]);
}

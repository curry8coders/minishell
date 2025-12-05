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

/**
 * Read a single heredoc line from the user and write it to the provided pipe.
 *
 * @param pfd Write-end file descriptor of the pipe where the line and a
 *            trailing newline will be written.
 * @param delimiter The heredoc delimiter string that terminates input when
 *                  entered on a line by itself.
 * @param is_delim_unquoted If `true`, apply heredoc expansions to the read
 *                          line before writing it.
 * @returns `true` if a line was successfully read (and written) and heredoc
 *          input should continue; `false` if EOF, the delimiter was entered,
 *          or a readline interruption occurred.
 */
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

/**
 * Create a pipe populated with lines read from a heredoc until the specified delimiter is entered.
 *
 * @param delimiter Delimiter string that, when entered on its own line, terminates heredoc input.
 * @param is_delim_unquoted If `true`, apply expansion to each heredoc line before writing it into the pipe.
 * @returns Read end file descriptor of the pipe containing the heredoc contents, or `-1` if the heredoc was interrupted.
 */
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
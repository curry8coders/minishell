/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/06 04:07:51 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include "minishell.h"

static bool	process_heredoc_line(t_shell *sh, int pfd, const char *delim,
	bool is_delim_unquoted)
{
	char	*line;

	line = readline("> ");
	if (line == NULL)
		return (false);
	if (sh->readline_interrupted)
	{
		free(line);
		return (false);
	}
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (false);
	}
	if (is_delim_unquoted)
		line = expand_heredoc_line(sh, line);
	write(pfd, line, ft_strlen(line));
	write(pfd, "\n", 1);
	free(line);
	return (true);
}

int	read_heredoc(t_shell *sh, const char *delim, bool is_delim_unquoted)
{
	int		pfd[2];

	if (pipe(pfd) < 0)
	{
		xperror("pipe");
		return (-1);
	}
	sh->readline_interrupted = false;
	while (process_heredoc_line(sh, pfd[1], delim, is_delim_unquoted))
		;
	close(pfd[1]);
	if (sh->readline_interrupted)
	{
		close(pfd[0]);
		return (-1);
	}
	return (pfd[0]);
}

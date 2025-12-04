/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/04 22:38:25 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include "minishell.h"

static void	skip_single_quote(char **line, char **rest)
{
	(*line)++;
	while (**line && **line != SINGLE_QUOTE_CHAR)
		(*line)++;
	if (**line == '\0')
	{
		tokenize_error("word(single quote)", rest, *line);
		return ;
	}
	(*line)++;
}

static void	skip_double_quote(char **line, char **rest)
{
	(*line)++;
	while (**line && **line != DOUBLE_QUOTE_CHAR)
		(*line)++;
	if (**line == '\0')
	{
		tokenize_error("word(double quote)", rest, *line);
		return ;
	}
	(*line)++;
}

t_token	*word(char **rest, char *line)
{
	const char	*start;
	char		*w;

	start = line;
	while (*line && !is_metacharacter(*line))
	{
		if (*line == SINGLE_QUOTE_CHAR)
			skip_single_quote(&line, rest);
		else if (*line == DOUBLE_QUOTE_CHAR)
			skip_double_quote(&line, rest);
		else
			line++;
		if (g_syntax_error)
			break ;
	}
	w = strndup(start, line - start);
	if (w == NULL)
		fatal_error("strndup");
	*rest = line;
	return (new_token(w, TK_WORD));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/06 08:04:50 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include "minishell.h"

bool	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

bool	consume_blank(char **rest, char *line)
{
	if (is_blank(*line))
	{
		while (*line && is_blank(*line))
			line++;
		*rest = line;
		return (true);
	}
	*rest = line;
	return (false);
}

bool	startswith(const char *s, const char *keyword)
{
	return (memcmp(s, keyword, strlen(keyword)) == 0);
}

bool	is_metacharacter(char c)
{
	if (is_blank(c))
		return (true);
<<<<<<< Updated upstream
	return (c && strchr("|&;()<>\n", c));
=======
	return (c && strchr("<>|\n", c));
>>>>>>> Stashed changes
}

bool	is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}

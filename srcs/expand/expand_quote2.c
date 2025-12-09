/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quote2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kenakamu <kenakamu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 03:43:52 by kenakamu          #+#    #+#             */
/*   Updated: 2025/12/10 03:44:10 by kenakamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_literal_quotes_tok(t_token *tok)
{
	char	*p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	while (*p)
	{
		if (*p == LITERAL_SINGLE_QUOTE)
			*p = SINGLE_QUOTE_CHAR;
		else if (*p == LITERAL_DOUBLE_QUOTE)
			*p = DOUBLE_QUOTE_CHAR;
		p++;
	}
	restore_literal_quotes_tok(tok->next);
}

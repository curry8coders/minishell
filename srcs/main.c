/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:10:51 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/30 20:33:27 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

int	g_last_status;

void	interpret(char *line, int *stat_loc)
{
	t_token	*tok;
	t_node	*node;

	tok = tokenize(line);
	if (syntax_error || at_eof(tok))
	{
		if (syntax_error)
			*stat_loc = ERROR_TOKENIZE;
		free_tok(tok);
		return;
	}
	
	node = parse(tok);
	if (syntax_error)
	{
		*stat_loc = ERROR_PARSE;
		free_node(node);
		free_tok(tok);
		return;
	}
	
	expand(node);
	if (syntax_error)
	{
		*stat_loc = ERROR_EXPAND;
		free_node(node);
		free_tok(tok);
		return;
	}
	
	*stat_loc = exec(node);
	free_node(node);
	free_tok(tok);
}

//*stat_locではメモリアクセスでステータス渡す
//status localtion
// なぜinterpretでexecが起動されるか

int	main(void)
{
	char	*line;

	rl_outstream = stderr;
	initenv();
	setup_signal();
	g_last_status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		interpret(line, &g_last_status);
		free(line);
	}
	exit(g_last_status);
}
//interpret(line, &status)は&アドレス渡し

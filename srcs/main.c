/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:10:51 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/22 07:36:04 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

int	last_status;

void interpret(char *line, int *stat_loc)
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
	last_status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		interpret(line, &last_status);
		free(line);
	}
	exit(last_status);
}
//interpret(line, &status)は&アドレス渡し

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:10:51 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/03 21:27:43 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

int	g_last_status;

/**
 * Process a single input line through tokenization, parsing, expansion, and execution,
 * and store the resulting exit or error status at the provided location.
 *
 * The function updates *stat_loc with one of the specific error codes
 * (ERROR_TOKENIZE, ERROR_PARSE, ERROR_EXPAND) if a failure occurs during the
 * corresponding stage, or with the value returned by exec() on success.
 * Internal token and syntax-node structures are freed before any return.
 *
 * @param line Input command line to interpret.
 * @param stat_loc Pointer to an integer where the final exit or error status will be stored.
 */
void	interpret(char *line, int *stat_loc)
{
	t_token	*tok;
	t_node	*node;

	tok = tokenize(line);
	if (g_syntax_error || at_eof(tok))
	{
		if (g_syntax_error)
			*stat_loc = ERROR_TOKENIZE;
		free_tok(tok);
		return;
	}
	
	node = parse(tok);
	if (g_syntax_error)
	{
		*stat_loc = ERROR_PARSE;
		free_node(node);
		free_tok(tok);
		return;
	}
	
	expand(node);
	if (g_syntax_error)
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
/**
 * Initialize the shell environment and run the Read–Eval–Print Loop until input ends, then exit with the last command's status.
 *
 * Sets up readline output, the environment, and signal handlers, then repeatedly reads lines, records history for non-empty lines, processes each line via interpret, and terminates the process with the most recent command status.
 *
 * @returns Process exit status equal to the most recent command's status.
 */

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
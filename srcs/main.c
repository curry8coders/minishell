/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:10:51 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/04 22:43:29 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

int	g_last_status;

static void	exec_and_cleanup(t_node *node, t_token *tok, int *stat_loc)
{
	expand(node);
	if (g_syntax_error)
	{
		*stat_loc = ERROR_EXPAND;
		free_node(node);
		free_tok(tok);
		return ;
	}
	*stat_loc = exec(node);
	free_node(node);
	free_tok(tok);
}

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
		return ;
	}
	node = parse(tok);
	if (g_syntax_error)
	{
		*stat_loc = ERROR_PARSE;
		free_node(node);
		free_tok(tok);
		return ;
	}
	exec_and_cleanup(node, tok, stat_loc);
}

int	main(void)
{
	char	*line;

	rl_outstream = stderr;
	initenv();
	setup_signal();
	g_last_status = 0;
	while (1)
	{
		g_readline_interrupted = false;
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (g_readline_interrupted)
		{
			g_last_status = 128 + SIGINT;
			free(line);
			continue ;
		}
		if (*line)
			add_history(line);
		interpret(line, &g_last_status);
		free(line);
	}
	write(STDERR_FILENO, "exit\n", 5);
	exit(g_last_status);
}

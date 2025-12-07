/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:10:51 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/07 18:54:55 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

static void	exec_and_cleanup(t_shell *sh, t_node *node, t_token *tok)
{
	expand(sh, node);
	if (sh->syntax_error)
	{
		sh->last_status = ERROR_EXPAND;
		free_node(node);
		free_tok(tok);
		return ;
	}
	sh->last_status = exec(sh, node);
	free_node(node);
	free_tok(tok);
}

static void	interpret(t_shell *shell, char *line)
{
	t_token	*tok;
	t_node	*node;

	tok = tokenize(shell, line);
	if (shell->syntax_error || at_eof(tok))
	{
		if (shell->syntax_error)
			shell->last_status = ERROR_TOKENIZE;
		free_tok(tok);
		return ;
	}
	node = parse(shell, tok);
	if (shell->syntax_error)
	{
		shell->last_status = ERROR_PARSE;
		free_node(node);
		free_tok(tok);
		return ;
	}
	exec_and_cleanup(shell, node, tok);
}

static void	init_shell(t_shell *shell)
{
	shell->last_status = 0;
	shell->exit_status = 0;
	shell->syntax_error = false;
	shell->readline_interrupted = false;
	shell->envmap = NULL;
}

static void	handle_line(t_shell *shell, char *line)
{
	if (line == NULL)
		return ;
	if (shell->readline_interrupted)
	{
		shell->last_status = 128 + SIGINT;
		free(line);
		return ;
	}
	if (*line)
		add_history(line);
	interpret(shell, line);
	free(line);
}

int	main(void)
{
	t_shell	shell;
	char	*line;

	rl_outstream = stderr;
	init_shell(&shell);
	initenv(&shell);
	setup_signal(&shell);
	while (1)
	{
		shell.readline_interrupted = false;
		line = readline("minishell$ ");
		if (g_sig == SIGINT)
		{
			shell.readline_interrupted = true;
			g_sig = 0;
		}
		if (line == NULL)
			break ;
		handle_line(&shell, line);
	}
	write(STDERR_FILENO, "exit\n", 5);
	exit(shell.last_status);
}

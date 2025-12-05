/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:36:01 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/06 03:44:19 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

t_node	*parse(t_shell *shell, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_PIPELINE);
	node->inpipe[0] = STDIN_FILENO;
	node->inpipe[1] = -1;
	node->outpipe[0] = -1;
	node->outpipe[1] = STDOUT_FILENO;
	node->command = simple_command(shell, &tok, tok);
	if (equal_op(tok, "|"))
		node->next = parse(shell, tok->next);
	return (node);
}

bool	is_control_operator(t_token *tok)
{
	static char *const	operators[] = {"|", "\n"};
	size_t				i;

	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(tok->word, operators[i]))
			return (true);
		i++;
	}
	return (false);
}

t_node	*simple_command(t_shell *shell, t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_SIMPLE_CMD);
	append_cmd_elm(shell, node, &tok, tok);
	while (tok && !at_eof(tok) && !is_control_operator(tok))
		append_cmd_elm(shell, node, &tok, tok);
	*rest = tok;
	return (node);
}

void	append_cmd_elm(t_shell *sh, t_node *cmd, t_token **rest, t_token *tok)
{
	if (tok->kind == TK_WORD)
	{
		append_tok(&cmd->args, tokdup(tok));
		tok = tok->next;
	}
	else if (equal_op(tok, ">") && tok->next->kind == TK_WORD)
		append_node(&cmd->redirects, redirect_out(&tok, tok));
	else if (equal_op(tok, "<") && tok->next->kind == TK_WORD)
		append_node(&cmd->redirects, redirect_input(&tok, tok));
	else if (equal_op(tok, ">>") && tok->next->kind == TK_WORD)
		append_node(&cmd->redirects, redirect_append(&tok, tok));
	else if (equal_op(tok, "<<") && tok->next->kind == TK_WORD)
		append_node(&cmd->redirects, redirect_heredoc(&tok, tok));
	else
	{
		parse_error(sh, &tok, tok);
		*rest = tok;
		return ;
	}
	*rest = tok;
}

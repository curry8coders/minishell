/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:36:01 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/30 19:40:45 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

/**
 * Construct a pipeline AST node from the token stream, including its simple command
 * and any subsequent piped commands.
 * @param tok Pointer to the current token where parsing of the pipeline should begin.
 * @returns Pointer to a `t_node` of kind `ND_PIPELINE` representing the parsed pipeline.
 */
t_node	*parse(t_token *tok)
{
	t_node	*node;

	node = new_node(ND_PIPELINE);
	node->inpipe[0] = STDIN_FILENO;
	node->inpipe[1] = -1;
	node->outpipe[0] = -1;
	node->outpipe[1] = STDOUT_FILENO;
	node->command = simple_command(&tok, tok);
	if (equal_op(tok, "|"))
		node->next = parse(tok->next);
	return (node);
}

/**
 * Determine whether the token's word begins with a shell control operator.
 *
 * @param tok Token whose word is tested against control operator prefixes.
 * @returns `true` if `tok->word` starts with any of the control operators
 *          ("||", "&", "&&", ";", ";;", "(", ")", "|", "\n"), `false` otherwise.
 */
bool	is_control_operator(t_token *tok)
{
	static char *const	operators[] = {"||", "&", "&&",
		";", ";;", "(", ")", "|", "\n"};
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

/**
 * Build a simple command node by consuming consecutive non-control tokens.
 *
 * Constructs an ND_SIMPLE_CMD node populated with argument and redirection elements
 * parsed from the token stream starting at `tok`. Parsing continues until the end
 * of input or a control operator is encountered. On return `*rest` is set to the
 * first token after the parsed command.
 *
 * @param rest Pointer to store the token at which parsing stopped (first token after the command).
 * @param tok  The starting token for the simple command.
 * @returns Pointer to an allocated ND_SIMPLE_CMD node representing the parsed command.
 */
t_node	*simple_command(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_SIMPLE_CMD);
	append_command_element(node, &tok, tok);
	while (tok && !at_eof(tok) && !is_control_operator(tok))
		append_command_element(node, &tok, tok);
	*rest = tok;
	return (node);
}

/**
 * Append a token's semantic element (argument or redirection) to a simple command node.
 *
 * Processes the token at `tok` and mutates `command` by either adding an argument
 * element or adding a redirection element. Recognized redirections are ">", "<",
 * ">>", and "<<" only when followed by a word token; otherwise the function
 * delegates to the unimplemented handler. On completion `*rest` is set to the
 * next token to be parsed (the first token not consumed by this call).
 *
 * @param command The simple-command node to which the argument or redirect is appended.
 * @param rest Out-parameter updated to point at the next unconsumed token after processing.
 * @param tok The current token to process.
 */
void	append_command_element(t_node *command, t_token **rest, t_token *tok)
{
	if (tok->kind == TK_WORD)
	{
		append_tok(&command->args, tokdup(tok));
		tok = tok->next;
	}
	else if (equal_op(tok, ">") && tok->next->kind == TK_WORD)
		append_node(&command->redirects, redirect_out(&tok, tok));
	else if (equal_op(tok, "<") && tok->next->kind == TK_WORD)
		append_node(&command->redirects, redirect_input(&tok, tok));
	else if (equal_op(tok, ">>") && tok->next->kind == TK_WORD)
		append_node(&command->redirects, redirect_append(&tok, tok));
	else if (equal_op(tok, "<<") && tok->next->kind == TK_WORD)
		append_node(&command->redirects, redirect_heredoc(&tok, tok));
	else
		todo("append_command_element");
	*rest = tok;
}
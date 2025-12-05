/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:57:17 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/06 03:43:07 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include "minishell.h"

t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*tok;

	tok = ft_calloc(1, sizeof(*tok));
	if (tok == NULL)
		fatal_error("ft_calloc");
	tok->word = word;
	tok->kind = kind;
	return (tok);
}

t_token	*operator(char **rest, char *line)
{
	static const char *const	operators[] = {">>", "<<",
		"<", ">", "|", "\n"};
	size_t						i;
	char						*op;

	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(line, operators[i]))
		{
			op = ft_strdup(operators[i]);
			if (op == NULL)
				fatal_error("ft_strdup");
			*rest = line + ft_strlen(op);
			return (new_token(op, TK_OP));
		}
		i++;
	}
	assert_error("Unexpected operator");
}

t_token	*tokenize(t_shell *shell, char *line)
{
	t_token	head;
	t_token	*tok;

	shell->syntax_error = false;
	head.next = NULL;
	tok = &head;
	while (*line && shell->syntax_error == false)
	{
		if (consume_blank(&line, line))
			continue ;
		else if (is_metacharacter(*line))
		{
			tok->next = operator(&line, line);
			tok = tok->next;
		}
		else if (is_word(line))
		{
			tok->next = word(shell, &line, line);
			tok = tok->next;
		}
		else
			assert_error("Unexpected Token");
	}
	tok->next = new_token(NULL, TK_EOF);
	return (head.next);
}

char	**tail_recursive(t_token *tok, int nargs, char **argv)
{
	char	**new_argv;

	if (tok == NULL || tok->kind == TK_EOF)
		return (argv);
	new_argv = realloc(argv, (nargs + 2) * sizeof(char *));
	if (new_argv == NULL)
	{
		free(argv);
		fatal_error("realloc");
	}
	argv = new_argv;
	argv[nargs] = ft_strdup(tok->word);
	if (argv[nargs] == NULL)
		fatal_error("ft_strdup");
	argv[nargs + 1] = NULL;
	return (tail_recursive(tok->next, nargs + 1, argv));
}

char	**token_list_to_argv(t_token *tok)
{
	char	**argv;

	argv = ft_calloc(1, sizeof(char *));
	if (argv == NULL)
		fatal_error("ft_calloc");
	return (tail_recursive(tok, 0, argv));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 20:51:03 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/15 04:29:22 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

#include <stdio.h>

void free_node(t_node *node)
{
	if (node == NULL)
		return ;
	free_tok(node->args);
	free_node(node->next);
	free_tok(node->filename);
free_tok(node->delimiter);
	free_node(node->redirects);
	free(node);
}

void free_tok(t_token *tok)
{
	if (tok == NULL)
		return ;
	if (tok->word)
		free(tok->word);
	free_tok(tok->next);
	free(tok);
}

void free_argv(char **argv)
{
	int i;

	if (argv == NULL)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

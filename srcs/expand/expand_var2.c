/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kenakamu <kenakamu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 03:44:03 by kenakamu          #+#    #+#             */
/*   Updated: 2025/12/10 03:44:07 by kenakamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_variable_node(t_shell *shell, t_node *node)
{
	if (node == NULL)
		return ;
	expand_variable_tok(shell, node->args);
	expand_variable_tok(shell, node->filename);
	expand_variable_node(shell, node->redirects);
	expand_variable_node(shell, node->command);
	expand_variable_node(shell, node->next);
}

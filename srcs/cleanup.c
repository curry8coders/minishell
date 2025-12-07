/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/07 16:54:40 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

void	free_map(t_map *map)
{
	t_item	*cur;
	t_item	*next;

	if (map == NULL)
		return ;
	cur = map->item_head.next;
	while (cur)
	{
		next = cur->next;
		free(cur->name);
		free(cur->value);
		free(cur);
		cur = next;
	}
	free(map);
}

void	cleanup_shell(t_shell *shell)
{
	if (shell == NULL)
		return ;
	if (shell->envmap)
	{
		free_map(shell->envmap);
		shell->envmap = NULL;
	}
	rl_clear_history();
}

void	exit_shell(t_shell *shell, int status)
{
	cleanup_shell(shell);
	exit(status);
}

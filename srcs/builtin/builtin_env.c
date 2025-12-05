/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 13:46:09 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/29 11:24:47 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>

int	builtin_env(t_shell *shell, char **argv)
{
	t_item	*cur;

	(void)argv;
	cur = shell->envmap->item_head.next;
	while (cur)
	{
		if (cur->value)
			printf("%s=%s\n", cur->name, cur->value);
		cur = cur->next;
	}
	printf("_=/usr/bin/env\n");
	return (0);
}

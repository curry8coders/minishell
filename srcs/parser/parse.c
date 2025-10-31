/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 20:41:49 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/10/31 20:43:37 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../includes/minishell.h"

#include <string.h>

t_node *parse(t_token *tok)
{
	t_node *node;
	
	node = new_node(ND_SIMPLE_CMD);
	
}
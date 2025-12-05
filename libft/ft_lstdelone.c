/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 03:27:59 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 12:16:55 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Delete a single list node and free its content using the provided deleter.
 *
 * If `lst` or `dsl` is NULL the function returns without performing any action.
 *
 * @param lst Pointer to the list node to delete.
 * @param dsl Function that will be called with the node's `content` to free or clean it up.
 */
void	ft_lstdelone(t_list *lst, void (*dsl)(void*))
{
	if (!lst || !dsl)
		return ;
	dsl(lst->content);
	free(lst);
}
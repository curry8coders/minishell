/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 03:30:53 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 12:19:48 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Delete and free all nodes of a linked list and set the head pointer to NULL.
 *
 * Applies the function `del` to the content of each node, frees each node's memory,
 * and advances until the list is empty. If `lst` or `del` is NULL, the function does nothing.
 *
 * @param lst Pointer to the head pointer of the list to clear.
 * @param del Function used to free the content of each node.
 */
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
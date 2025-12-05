/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 03:17:58 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 12:04:21 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Append a node to the end of a singly linked list.
 *
 * If `lst` or `new` is NULL the function does nothing. If the list is empty
 * (`*lst` is NULL) the new node becomes the head; otherwise `new` is linked
 * as the last node's `next`.
 *
 * @param lst Pointer to the head pointer of the list.
 * @param new Node to append to the list.
 */
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
}
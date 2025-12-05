/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 02:59:39 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 12:03:00 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Insert a node at the front of a singly linked list.
 *
 * If `lst` or `new` is NULL, the function does nothing. Otherwise `new` becomes
 * the new head and its `next` pointer is set to the previous head.
 *
 * @param lst Pointer to the head pointer of the list.
 * @param new Node to insert at the front of the list.
 */
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}
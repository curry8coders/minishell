/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 03:12:02 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 12:03:40 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Count the number of elements in a singly linked list.
 *
 * Iterates the list starting at `lst` and returns the total node count.
 * If `lst` is NULL, returns 0.
 *
 * @param lst Pointer to the head of the list (may be NULL).
 * @return The number of nodes in the list.
 */
int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
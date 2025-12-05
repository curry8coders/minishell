/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 04:04:17 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/17 16:24:13 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Create a new linked list by applying `f` to the content of each node in `lst`.
 *
 * Applies `f` to each node's `content`, creates a new node for the transformed
 * content, and links those nodes into a newly allocated list. If allocation of
 * any new node fails, `del` is used to free the content of all already-created
 * nodes and the function returns `NULL`.
 *
 * @param lst Head of the input list to iterate.
 * @param f Function applied to each node's content; its return value becomes the content of the new node.
 * @param del Function used to free a node's content when cleaning up after an allocation failure.
 * @returns Pointer to the head of the newly created list, or `NULL` if an allocation fails.
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void(*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;
	t_list	*last;

	new_list = NULL;
	last = new_list;
	while (lst)
	{
		new_node = ft_lstnew(f(lst->content));
		if (!new_node)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		if (!new_list)
			new_list = new_node;
		else
		{
			while (last->next)
				last = last->next;
			last->next = new_node;
		}
		lst = lst->next;
	}
	return (new_list);
}
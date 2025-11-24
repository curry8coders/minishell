/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:01:54 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 02:31:26 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*tmp;
	const char	*s;

	if (dest <= src)
	{
		tmp = (char *)dest;
		s = (char *)src;
		while (n--)
			*tmp++ = *s++;
	}
	else
	{
		tmp = (char *)dest;
		tmp += n;
		s = (char *)src;
		s += n;
		while (n--)
			*--tmp = *--s;
	}
	return (dest);
}

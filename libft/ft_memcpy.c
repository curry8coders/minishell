/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 03:49:14 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/17 19:13:38 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//void	*ft_memcpy(void *dest, const void *src, size_t n)
//{
//	size_t			i;
//	unsigned char	*dest_c;
//	unsigned char	*src_c;
//
//	i = 0;
//	dest_c = (unsigned char *)dest;
//	src_c = (unsigned char *)src;
//	if (dest == NULL || src == NULL)
//		return (NULL);
//	while (i < n)
//	{
//		dest_c[i] = src_c[i];
//		i++;
//	}
//	return (dest);
//}
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

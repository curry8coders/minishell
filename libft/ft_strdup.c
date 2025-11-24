/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:05:43 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 02:40:08 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	while (*src++ != '\0')
		*dest++ = *src++;
	return (dest);
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*dup;

	dup = (char *) malloc(ft_strlen(s) + 1);
	if (!dup)
		return (NULL);
	len = 0;
	while (s[len])
	{
		dup[len] = s[len];
		len++;
	}
	dup[len] = '\0';
	return (dup);
}

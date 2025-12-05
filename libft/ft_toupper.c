/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 00:09:55 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/06 05:16:02 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Convert a lowercase ASCII letter to its uppercase equivalent.
 *
 * @param c The character to convert, passed as an int. Only values corresponding to ASCII lowercase letters ('a'..'z') are changed.
 * @returns The uppercase equivalent if `c` is a lowercase ASCII letter; otherwise returns `c` unchanged.
 */
int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c = c - 32;
	return (c);
}
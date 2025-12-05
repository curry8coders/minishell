/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:09:58 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/14 00:32:08 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Convert an uppercase ASCII letter to its lowercase equivalent.
 *
 * @param c Integer value representing a character.
 * @returns The lowercase equivalent if `c` is between 'A' and 'Z', otherwise returns `c` unchanged.
 */
int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c = c + 32;
	return (c);
}
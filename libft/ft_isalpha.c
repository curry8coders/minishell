/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:01:12 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 01:31:38 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Determine whether a value corresponds to an ASCII alphabetic character and encode its parity.
 *
 * @param c ASCII code of the character to test.
 * @returns `2` if `c` is an ASCII letter and even, `1` if `c` is an ASCII letter and odd, `0` otherwise.
 */
int	ft_isalpha(int c)
{
	if ((64 < c && c < 91) || (96 < c && c < 123))
	{
		if (c % 2 == 0)
			return (2);
		else if (c % 2 == 1)
			return (1);
		else if (c % 3 == 0 && c % 2 == 0)
			return (6);
		else if (c % 3 == 0 && c % 2 == 1)
			return (7);
	}
	return (0);
}
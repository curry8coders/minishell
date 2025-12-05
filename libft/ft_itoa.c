/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 23:06:46 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 11:47:08 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Compute the number of characters required to represent an integer in base 10,
 * including a minus sign for negative values and a digit for zero.
 * @param n Integer to measure.
 * @returns Number of characters required to represent `n` (does not include the null terminator).
 */
static	int	ft_numlen(int n)
{
	int		len;
	long	nb;

	nb = n;
	len = 0;
	if (nb <= 0)
	{
		len++;
		nb = -nb;
	}
	while (nb)
	{
		len++;
		nb /= 10;
	}
	return (len);
}

/**
 * Convert an integer to a null-terminated decimal string.
 *
 * @param n Integer value to convert.
 * @returns Pointer to a newly allocated null-terminated string containing the base-10
 * representation of `n` (including a leading '-' for negative values), or `NULL` if
 * memory allocation fails.
 */
char	*ft_itoa(int n)
{
	char	*str;
	long	nb;
	int		len;

	nb = n;
	len = ft_numlen(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (nb == 0)
		str[0] = '0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		str[len--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (str);
}
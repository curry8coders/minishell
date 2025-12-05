/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 08:03:55 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/29 00:30:12 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Selects the appropriate integer overflow boundary for the given sign.
 * @param sign If 1, indicates a positive value; any other value is treated as negative.
 * @returns `INT_MAX` when `sign` is 1, `INT_MIN` otherwise.
 */
int	ft_handle_overflow(int sign)
{
	if (sign == 1)
		return (INT_MAX);
	return (INT_MIN);
}

/**
 * Convert the initial portion of a string to an int.
 *
 * Parses optional leading whitespace, an optional '+' or '-' sign, and consecutive decimal digits; parsing stops at the first non-digit character.
 * @param nptr String to convert; may contain leading whitespace and an optional '+' or '-' sign.
 * @returns INT_MAX if overflow occurs and the sign is positive, INT_MIN if overflow occurs and the sign is negative, otherwise the parsed integer value.
 */
int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (nptr[i] == ' ' || nptr[i] == '\t' || nptr[i] == '\n'
		|| nptr[i] == '\v' || nptr[i] == '\f'
		|| nptr[i] == '\r')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (result > (INT_MAX - (nptr[i] - '0')) / 10)
			return (ft_handle_overflow(sign));
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (sign * result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 15:10:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/06 15:17:26 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>
#include <errno.h>

static void	skip_whitespace(const char **str)
{
	while (**str == ' ' || **str == '\t' || **str == '\n'
		|| **str == '\v' || **str == '\f' || **str == '\r')
		(*str)++;
}

static int	get_sign(const char **str)
{
	int	sign;

	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}

static int	check_overflow(long result, int digit, int sign, long *out)
{
	if (sign == 1 && (result > LONG_MAX / 10
			|| (result == LONG_MAX / 10 && digit > LONG_MAX % 10)))
	{
		errno = ERANGE;
		*out = LONG_MAX;
		return (1);
	}
	if (sign == -1 && (result > -(LONG_MIN / 10)
			|| (result == -(LONG_MIN / 10) && digit > -(LONG_MIN % 10))))
	{
		errno = ERANGE;
		*out = LONG_MIN;
		return (1);
	}
	return (0);
}

static long	parse_digits(const char **str, int sign)
{
	long	result;
	long	overflow_val;
	int		digit;

	result = 0;
	while (ft_isdigit(**str))
	{
		digit = **str - '0';
		if (check_overflow(result, digit, sign, &overflow_val))
			return (overflow_val);
		result = result * 10 + digit;
		(*str)++;
	}
	return (result * sign);
}

long	ft_strtol(const char *str, char **endptr, int base)
{
	int		sign;
	long	result;

	if (base != 10)
		return (0);
	skip_whitespace(&str);
	sign = get_sign(&str);
	if (!ft_isdigit(*str))
	{
		if (endptr)
			*endptr = (char *)str;
		return (0);
	}
	result = parse_digits(&str, sign);
	if (endptr)
		*endptr = (char *)str;
	return (result);
}

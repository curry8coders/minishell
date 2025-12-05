/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 06:17:35 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 01:36:39 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Check whether a character code is a decimal digit.
 *
 * @param c Character code to test (typically an unsigned char cast to int or EOF).
 * @return `1` if `c` is a digit between `'0'` and `'9'`, `0` otherwise.
 */
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
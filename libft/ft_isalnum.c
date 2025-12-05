/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 06:22:51 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 01:43:00 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Check if a character is alphanumeric (alphabetic letter or digit).
 *
 * @param c Character value to test, passed as an int (typically `EOF` or a value representable as `unsigned char`).
 * @returns Nonzero if `c` is an alphabetic letter (`A`–`Z`, `a`–`z`) or a digit (`0`–`9`), 0 otherwise.
 */
int	ft_isalnum(int c)
{
	return ((ft_isalpha(c) || ft_isdigit(c)));
}
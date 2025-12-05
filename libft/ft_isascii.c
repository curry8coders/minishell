/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:29:23 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/14 14:06:10 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Determine whether an integer value represents an ASCII character.
 *
 * @param c Integer value to test.
 * @returns `1` if `c` is within the ASCII range 0–127, `0` otherwise.
 */
int	ft_isascii(int c)
{
	return (0 <= c && c <= 127);
}
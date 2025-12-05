/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 08:55:51 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/14 14:07:37 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Check whether a character code corresponds to a printable ASCII character.
 *
 * @param c Integer value of a character (typically an unsigned char cast to int).
 * @returns `1` if `c` is a printable ASCII character (code 32 through 126), `0` otherwise.
 */
int	ft_isprint(int c)
{
	return (c >= 32 && c <= 126);
}
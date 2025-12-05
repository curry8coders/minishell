/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 00:32:45 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 11:49:18 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Write the decimal representation of an integer to the given file descriptor.
 *
 * If n equals INT_MIN (-2147483648), the literal string "-2147483648" is written.
 * For other negative values a leading '-' is written before the digits.
 * Digits are written in base 10 with no trailing newline.
 * @param n Integer value to output.
 * @param fd Target file descriptor. 
 */
void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n > 9)
		ft_putnbr_fd(n / 10, fd);
	c = n % 10 + '0';
	write(fd, &c, 1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ac_put_uniques.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kenakamu <kenakamu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 01:49:43 by kenakamu          #+#    #+#             */
/*   Updated: 2025/08/14 01:52:05 by kenakamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_put_un_fd(int fd, unsigned int n)
{
	int	len;

	len = 0;
	if (n / 10)
	{
		len = ft_put_un_fd(fd, n / 10);
		if (len == -1)
			return (-1);
	}
	if (ft_putchar_fd(fd, (n % 10) + '0') == -1)
		return (-1);
	len++;
	return (len);
}

int	ft_put_hex_fd(int fd, uintptr_t value, int islower)
{
	char	*hex_digits;
	int		i;
	char	buffer[17];

	if (islower)
		hex_digits = "0123456789abcdef";
	else
		hex_digits = "0123456789ABCDEF";
	i = 16;
	buffer[i--] = '\0';
	if (value == 0)
		buffer[i--] = '0';
	else
	{
		while (value > 0)
		{
			buffer[i--] = hex_digits[value % 16];
			value = value / 16;
		}
	}
	if (ft_putstr_fd(fd, &buffer[i + 1]) == -1)
		return (-1);
	return (16 - i - 1);
}

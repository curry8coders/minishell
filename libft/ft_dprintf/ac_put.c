/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ac_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kenakamu <kenakamu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 01:49:41 by kenakamu          #+#    #+#             */
/*   Updated: 2025/08/14 01:49:42 by kenakamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_putchar_fd(int fd, char c)
{
	return (write(fd, &c, 1));
}

int	ft_putstr_fd(int fd, char *str)
{
	size_t	len;

	len = 0;
	if (str == NULL)
		str = "(null)";
	while (*str)
	{
		if (ft_putchar_fd(fd, *str) == -1)
			return (-1);
		len++;
		str++;
	}
	return (len);
}

int	ft_put_ptr_fd(int fd, void *ptr)
{
	int	len;
	int	hex_len;

	len = 0;
	if (ptr == NULL)
	{
		if (ft_putstr_fd(fd, "(nil)") == -1)
			return (-1);
		return (5);
	}
	if (ft_putstr_fd(fd, "0x") == -1)
		return (-1);
	len = 2;
	hex_len = ft_put_hex_fd(fd, (uintptr_t)ptr, 1);
	if (hex_len == -1)
		return (-1);
	return (len + hex_len);
}

int	ft_putnbr_fd(int fd, int n)
{
	int	len;

	if (n == INT_MIN)
		return (ft_putstr_fd(fd, "-2147483648"));
	len = 0;
	if (n < 0)
	{
		if (ft_putchar_fd(fd, '-') == -1)
			return (-1);
		len++;
		n *= -1;
	}
	if (n / 10)
		len += ft_putnbr_fd(fd, n / 10);
	if (ft_putchar_fd(fd, '0' + (n % 10)) == -1)
		return (-1);
	len++;
	return (len);
}

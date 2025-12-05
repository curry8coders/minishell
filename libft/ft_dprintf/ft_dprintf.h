/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kenakamu <kenakamu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:07:39 by kenakamu          #+#    #+#             */
/*   Updated: 2025/05/11 14:19:17 by kenakamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H
# define FT_DPRINTF_H
# include <stdarg.h>

# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>
# include <limits.h>

int	ft_dprintf(int fd, const char *fmt, ...);

int	ft_dprintf_core(int fd, const char *fmt, va_list ap);
int	prss_placeholder(int fd, va_list ap, char prosess);

int	ft_putchar_fd(int fd, char c);
int	ft_putstr_fd(int fd, char *str);
int	ft_put_ptr_fd(int fd, void *ptr);
int	ft_putnbr_fd(int fd, int n);
int	ft_put_hex_fd(int fd, uintptr_t value, int islower);
int	ft_put_ptr_fd(int fd, void *ptr);
int	ft_put_un_fd(int fd, unsigned int n);

#endif

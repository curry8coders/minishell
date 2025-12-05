/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 00:08:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/29 03:32:22 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Apply a function to each character of a null-terminated string, passing
 * the character's index and a pointer to the character.
 *
 * If `s` or `f` is NULL, the function does nothing.
 *
 * @param s Pointer to the null-terminated string to modify.
 * @param f Function to call for each character; receives the character index
 *          and a pointer to the character to allow in-place modification.
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
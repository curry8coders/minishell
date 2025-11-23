/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:35:34 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 10:12:21 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//int	ft_strlen(const char *s)
//{
//	size_t	i;
//
//	i = 0;
//	while (*s != '\0')
//		i++;
//	return (i);
//}
//
//char	*ft_substr(char const *s, unsigned int start, size_t len)
//{
//	char	*substr;
//	size_t	i;
//	size_t	s_len;
//
//	if (!s)
//		return (NULL);
//	s_len = ft_strlen(s);
//	if (start >= s_len)
//	{
//		substr = (char *)malloc(1);
//		if (!substr)
//			return (NULL);
//		substr[0] = '\0';
//		return (substr);
//	}
//	if (s_len - start < len)
//		len = s_len - start;
//	substr = (char *)malloc(sizeof(char) * (len + 1));
//	if (!substr)
//		return (NULL);
//	i = 0;
//	while (i < len && s[start + i])
//	{
//		substr[i] = s[start + i];
//		i++;
//	}
//	substr[i] = '\0';
//	return (substr);
//}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	return (str);
}

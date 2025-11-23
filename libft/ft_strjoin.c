/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:11:08 by hichikaw          #+#    #+#             */
/*   Updated: 2024/11/16 10:13:58 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//size_t	ft_all_len(char const *s1, char const *s2)
//{
//	size_t	s1_len;
//	size_t	s2_len;
//
//	s1_len = 0;
//	s2_len = 0;
//	while (s1[s1_len])
//		s1_len++;
//	while (s2[s2_len])
//		s2_len++;
//	return (s1_len + s2_len + 1);
//}
//
//char	*ft_strjoin(char const *s1, char const *s2)
//{
//	char	*joined_str;
//	size_t	i;
//	size_t	j;
//
//	if (!s1 || !s2)
//		return (NULL);
//	joined_str = (char *)malloc(sizeof(char) * (ft_all_len(s1, s2)));
//	if (!joined_str)
//		return (NULL);
//	i = 0;
//	while (*s1)
//	{
//		joined_str[i] = s1[i];
//		i++;
//	}
//	j = 0;
//	while (*s2)
//	{
//		joined_str[i + j] = s2[j];
//		j++;
//	}
//	joined_str[i + j] = '\0';
//	return (joined_str);
//}
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined_str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	joined_str = (char *) malloc((ft_strlen(s1) + ft_strlen(s2) + 1)
			* sizeof(char));
	if (!joined_str)
		return (NULL);
	while (s1[i])
		joined_str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		joined_str[j++] = s2[i++];
	joined_str[j] = 0;
	return (joined_str);
}

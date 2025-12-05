/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_strbuf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/04 22:00:00 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

void	strbuf_init(t_strbuf *buf)
{
	buf->capacity = STRBUF_INIT_CAP;
	buf->data = malloc(buf->capacity);
	if (buf->data == NULL)
		fatal_error("malloc");
	buf->data[0] = '\0';
	buf->len = 0;
}

void	strbuf_grow(t_strbuf *buf, size_t needed)
{
	char	*new_data;
	size_t	new_cap;

	if (buf->len + needed < buf->capacity)
		return ;
	new_cap = buf->capacity * 2;
	while (buf->len + needed >= new_cap)
		new_cap *= 2;
	new_data = malloc(new_cap);
	if (new_data == NULL)
		fatal_error("malloc");
	ft_memcpy(new_data, buf->data, buf->len + 1);
	free(buf->data);
	buf->data = new_data;
	buf->capacity = new_cap;
}

void	strbuf_append_char(t_strbuf *buf, char c)
{
	strbuf_grow(buf, 2);
	buf->data[buf->len++] = c;
	buf->data[buf->len] = '\0';
}

void	strbuf_append_str(t_strbuf *buf, const char *s)
{
	size_t	slen;

	if (s == NULL)
		return ;
	slen = ft_strlen(s);
	strbuf_grow(buf, slen + 1);
	ft_memcpy(buf->data + buf->len, s, slen + 1);
	buf->len += slen;
}

char	*strbuf_finish(t_strbuf *buf)
{
	return (buf->data);
}

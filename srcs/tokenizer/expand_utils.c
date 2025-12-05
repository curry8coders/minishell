/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/04 22:35:55 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ctype.h>
#include "minishell.h"

bool	is_alpha_under(char c)
{
	return (isalpha(c) || c == '_');
}

bool	is_alpha_num_under(char c)
{
	return (is_alpha_under(c) || isdigit(c));
}

bool	is_variable(char *s)
{
	return (s[0] == '$' && is_alpha_under(s[1]));
}

bool	is_special_parameter(char *s)
{
	return (s[0] == '$' && s[1] == '?');
}

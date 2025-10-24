/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:57:43 by hichikaw          #+#    #+#             */
/*   Updated: 2025/10/24 18:24:42 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minishell.h"

#define SIGNAL_QUOTE_CHAR '\''

typedef struct s_token t_token;
enum e_token_kind
{
	TK_WORD,
	TK_RESERVED,
	TK_OP,
	TK_EOF,
};
typedef enum e_token_kind t_token_kind;

struct s_token
{
	char			*word;
	t_token_kind	kind;
	t_token		*next;
};

// tokenize
t_token *new_token(char *word, t_token_kind kind);
bool is_blank(char c);
bool consume_blank(char **rest, char *line);
bool is_operator(const char *s);
bool is_metacharacter(char c);
bool is_word(const char *s);
t_token *operator(char **rest, char *line);
t_token *word(char **rest, char *line);
t_token *tokenize(char *line);

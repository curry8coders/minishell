#include <string.h>

#include <stdlib.h>
#include "minishell.h"

bool	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}
// 本家bashではc == '\n'が含まれていない
// minishell \nを含めると:
// minishell$ echo a
// echo b
// a echo b
// 
// 修正 is_blankのみ修正:
// inishell$ echo a
// echo b
// a
// それだけでは治らないので要検討
// 
// 本家：
// $ echo a
// echo b
// a
// b
// 改行はコマンドを区切る「行の終わり」として扱われる
bool	consume_blank(char **rest, char *line)
{
	if (is_blank(*line))
	{
		while (*line && is_blank(*line))
			line++;
		*rest = line;
		return (true);
	}
	*rest = line;
	return (false);
}

bool	startswith(const char *s, const char *keyword)
{
	return (memcmp(s, keyword, strlen(keyword)) == 0);
}

bool	is_metacharacter(char c)
{
	if (is_blank(c))
		return (true);
	return (c && strchr("|&;()<>\n", c));
}

bool	is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}

t_token	*word(char **rest, char *line)
{
	const char	*start = line;
	char		*word;

	while (*line && !is_metacharacter(*line))
	{
		if (*line == SINGLE_QUOTE_CHAR)
		{
			line++;
			while (*line && *line != SINGLE_QUOTE_CHAR)
				line++;
			
			if (*line == '\0')
			{
				tokenize_error("word(single quote)", rest, line);//beta
				break;
			}
			line++;
		}
		else if (*line == DOUBLE_QUOTE_CHAR)
		{
			line++;
			while (*line && *line != DOUBLE_QUOTE_CHAR)
				line++;
			if (*line == '\0')
			{
				tokenize_error("word(double quote)", rest, line);//beta
				break ;
			}
			line++;
		}
		else
			line++;
	}
	word = strndup(start, line - start);
	if (word == NULL)
		fatal_error("strndup");
	*rest = line;
	return (new_token(word, TK_WORD));
}

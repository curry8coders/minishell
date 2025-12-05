#include <string.h>

#include <stdlib.h>
#include "minishell.h"

/**
 * Check whether a character is a blank (space or tab).
 *
 * @returns `true` if the character is a space `' '` or a tab `'\t'`, `false` otherwise.
 */
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
/**
 * Advance past leading space and tab characters in the given input line.
 *
 * If the character at `line` is a space or tab, sets `*rest` to point to the
 * first character after the consecutive blanks; otherwise sets `*rest` to
 * `line`.
 *
 * @param rest Pointer to receive the updated position within the line.
 * @param line Current position in the input line to examine.
 * @returns `true` if one or more blank characters were consumed, `false` otherwise.
 */
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

/**
 * Check whether the string `s` begins with the sequence `keyword`.
 *
 * @param s The string to inspect; must be a valid C string.
 * @param keyword The prefix to check for; must be a valid C string.
 * @returns `true` if `s` begins with `keyword`, `false` otherwise.
 */
bool	startswith(const char *s, const char *keyword)
{
	return (memcmp(s, keyword, strlen(keyword)) == 0);
}

/**
 * Determine whether a character is a metacharacter for tokenization.
 * @param c Character to classify.
 * @returns `true` if `c` is a space or tab or one of the characters `|&;()<>\n`, `false` otherwise.
 */
bool	is_metacharacter(char c)
{
	if (is_blank(c))
		return (true);
	return (c && strchr("|&;()<>\n", c));
}

/**
 * Determine whether the given string begins with a word character (not a metacharacter).
 *
 * @param s String to test; may be empty or NULL-terminated.
 * @returns `true` if `s` is not empty and its first character is not a metacharacter, `false` otherwise.
 */
bool	is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}

/**
 * Extracts the next word token from the input line, treating quoted segments as part of the word.
 *
 * Parses characters from `line` until a metacharacter is reached, consuming single-quoted or
 * double-quoted sections as part of the word. On an unmatched quote, calls `tokenize_error`
 * with a descriptive tag and stops scanning at the string end. On memory allocation failure,
 * calls `fatal_error`.
 *
 * @param rest Pointer that will be set to the position in the line immediately after the parsed word.
 * @param line Starting position in the input line to parse the word from.
 * @returns A newly allocated `TK_WORD` token containing the parsed word.
 */
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
#include <stdlib.h>
#include "minishell.h"
#include <string.h>
#include <ctype.h>

/**
 * Check whether a character is an alphabetic letter or an underscore.
 * @param c Character to test.
 * @returns `true` if `c` is an alphabetic character (A–Z, a–z) or `'_'`, `false` otherwise.
 */
bool	is_alpha_under(char c)
{
	return (isalpha(c) || c == '_');
}

/**
 * Determine whether a character is alphabetic, numeric, or an underscore.
 *
 * @param c Character to test.
 * @return `true` if `c` is an alphabetic character (A–Z or a–z), a digit (0–9), or an underscore (`_`), `false` otherwise.
 */
bool	is_alpha_num_under(char c)
{
	return (is_alpha_under(c) || isdigit(c));
}

/**
 * Check whether a string represents a simple shell variable name starting with '$' followed by a letter or underscore.
 * @param s Pointer to the null-terminated string to test.
 * @returns `true` if the string starts with `$` and the next character is an alphabetic character or `_`, `false` otherwise.
 */
bool	is_variable(char *s)
{
	return (s[0] == '$' && is_alpha_under(s[1]));
}

/**
 * Check whether a string represents the special shell parameter "$?".
 *
 * @param s NUL-terminated string to test; must point to at least two characters.
 * @returns `true` if the string starts with "$?", `false` otherwise.
 */
bool	is_special_parameter(char *s)
{
	return (s[0] == '$' && s[1] == '?');
}
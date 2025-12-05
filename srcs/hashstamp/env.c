/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 03:14:27 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/11/30 20:58:32 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include "minishell.h"

#include <string.h>

t_map	*g_envmap;

static void	envmap_init(t_map *map, char **ep);

/**
 * Retrieve the value of an environment variable from the global environment map.
 *
 * @param name Name of the environment variable to look up.
 * @return Pointer to the value string if found, NULL otherwise.
 */
char	*xgetenv(const char *name)
{
	return (map_get(g_envmap, name));
}

/**
 * Initialize the global environment map from the process environment.
 *
 * Populates the global pointer `g_envmap` with a new map containing the current
 * process environment variables (from `environ`). The resulting `g_envmap` is
 * ready for lookup and may include ensured entries such as `SHLVL`, `PWD`, and
 * `OLDPWD` when absent from the original environment.
 */
void	initenv(void)
{
	extern char	**environ;

	g_envmap = map_new();
	envmap_init(g_envmap, environ);
}

/**
 * Build a NULL-terminated array of environment strings from a map.
 *
 * Creates a newly allocated array containing "KEY=VALUE" strings for each
 * map entry that has a non-NULL value. The array is terminated with NULL.
 *
 * @param map Map containing environment entries; entries with NULL values are skipped.
 * @returns Pointer to the newly allocated NULL-terminated array of strings (each "KEY=VALUE").
 *          The caller is responsible for freeing the array and the strings it contains.
 */
char	**get_environ(t_map *map)
{
	size_t	i;
	size_t	size;
	t_item	*item;
	char	**environ;

	size = map_len(map, false) + 1;
	environ = calloc(size, sizeof(char *));
	i = 0;
	item = map->item_head.next;
	while (item)
	{
		if (item->value)
		{
			environ[i] = item_get_string(item);
			i++;
		}
		item = item->next;
	}
	environ[i] = NULL;
	return (environ);
}

static void	envmap_init(t_map *map, char **ep)
{
	char	cwd[PATH_MAX];

	while (*ep)
	{
		map_put(map, *ep, false);
		ep++;
	}
	if (map_get(map, "SHLVL") == NULL)
		map_set(map, "SHLVL", "1");
	if (map_get(map, "PWD") == NULL)
	{
		getcwd(cwd, PATH_MAX);
		map_set(map, "PWD", cwd);
	}
	if (map_get(map, "OLDPWD") == NULL)
		map_set(map, "OLDPWD", NULL);
}
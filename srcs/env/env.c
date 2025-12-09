/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 03:14:27 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/05 21:37:32 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

static void	envmap_init(t_map *map, char **ep);

char	*xgetenv(t_shell *shell, const char *name)
{
	return (map_get(shell->envmap, name));
}

void	initenv(t_shell *shell)
{
	extern char	**environ;

	shell->envmap = map_new();
	envmap_init(shell->envmap, environ);
}

char	**get_environ(t_map *map)
{
	size_t	i;
	size_t	size;
	t_item	*item;
	char	**environ;

	size = map_len(map, false) + 1;
	environ = ft_calloc(size, sizeof(char *));
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

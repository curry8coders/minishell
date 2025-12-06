/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:39:33 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/06 15:47:33 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

int	check_state(void)
{
	if (g_sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_done = 1;
		return (0);
	}
	return (0);
}

void	setup_signal(t_shell *shell)
{
	extern int	_rl_echo_control_chars;

	_rl_echo_control_chars = 0;
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))
	{
		rl_event_hook = check_state;
		ignore_sig(SIGQUIT);
	}
	setup_sigint();
	(void)shell;
}

void	reset_signal(void)
{
	reset_sig(SIGQUIT);
	reset_sig(SIGINT);
}

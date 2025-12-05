/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:00:00 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/30 20:20:05 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "minishell.h"

/**
 * Record the received signal number in the global signal variable.
 *
 * @param signum The signal number received (as delivered to the process). 
 */
void	handler(int signum)
{
	g_sig = signum;
}

/**
 * Restore the default action for the given signal.
 *
 * Configures `signum` to use the default signal handler (`SIG_DFL`) with an empty signal mask.
 *
 * @param signum Signal number to reset to the default action.
 *
 * @remark Calls fatal_error("sigaction") if `sigaction` fails. 
 */
void	reset_sig(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

/**
 * Configure the specified signal to be ignored.
 *
 * Sets the signal action for `signum` to `SIG_IGN` so that the signal is ignored.
 *
 * @param signum Signal number to be ignored.
 *
 * @note On failure, this function calls `fatal_error("sigaction")`.
 */
void	ignore_sig(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

/**
 * Install the program's SIGINT handler so that received SIGINT signals invoke `handler`.
 *
 * Configures SIGINT using `sigaction`; if `sigaction` fails, calls `fatal_error("sigaction")`.
 */
void	setup_sigint(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if (sigaction(SIGINT, &sa, NULL) < 0)
		fatal_error("sigaction");
}
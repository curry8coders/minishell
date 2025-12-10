/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 02:50:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/10 12:55:00 by agent           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"

void	handle_child_status(pid_t wait_result, pid_t last_pid,
		int *status, int wstatus)
{
	static int	signaled_newline_printed;

	if (wait_result == last_pid)
		signaled_newline_printed = 0;
	if (wait_result == last_pid)
		*status = get_exit_status(wstatus);
	if (WIFSIGNALED(wstatus) && !signaled_newline_printed)
	{
		write(STDERR_FILENO, "\n", 1);
		signaled_newline_printed = 1;
	}
}

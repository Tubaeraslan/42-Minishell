/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:07:21 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/02 12:19:28 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_signal_status = 130;
}

void	handle_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	child_signal_handler2(int sig)
{
	int	fd;

	if (sig == SIGINT)
	{
		g_signal_status = 130;
		fd = 3;
		while (fd < 1024)
		{
			close(fd);
			fd++;
		}
		signal(SIGINT, SIG_IGN);
	}
}

void	child_signal_handler(int sig)
{
	int	fd;

	if (sig == SIGINT)
	{
		g_signal_status = 130;
		write(1, "\n", 1);
		fd = 3;
		while (fd < 1024)
		{
			close(fd);
			fd++;
		}
		free_heredoc(NULL);
		exit(g_signal_status);
	}
}

void	set_signal(int i)
{
	if (i == 0)
		signal(SIGINT, child_signal_handler);
	if (i == 1)
		signal(SIGINT, child_signal_handler2);
}

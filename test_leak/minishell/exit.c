/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:16:12 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/03 12:45:27 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_code_from_status(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(1, "\r", 1);
			return (130);
		}
		else if (sig == SIGQUIT)
		{
			write(1, "Quit (core dumped)\n", 19);
			return (131);
		}
		else
			return (128 + sig);
	}
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	numeric_control(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (arg[i] == '\0')
		return (1);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	return (0);
}

void	close_all_fds(t_command *cmd)
{
	while (cmd)
	{
		if (cmd->in_fd > 2)
		{
			close(cmd->in_fd);
			cmd->in_fd = -1;
		}
		if (cmd->out_fd > 2)
		{
			close(cmd->out_fd);
			cmd->out_fd = -1;
		}
		if (cmd->heredoc_fd > 2)
		{
			close(cmd->heredoc_fd);
			cmd->heredoc_fd = -1;
		}
		cmd = cmd->next;
	}
}

void	exit_program(t_command *cmd, int exit_code)
{
	close_all_fds(cmd);
	if (cmd)
		all_free(cmd);
	rl_clear_history();
	exit(exit_code);
}

int	ft_exit(t_command *cmd)
{
	long	exit_code;

	write(2, "exit\n", 5);
	exit_control(cmd);
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (127);
	}
	if (cmd->args[1] && numeric_control(cmd->args[1]) == 1)
		exit_control2(cmd);
	if (cmd->args[1])
	{
		exit_code = ft_atoi(cmd->args[1]);
		if (exit_code < 0)
			exit_code = 256 + (exit_code % 256);
		exit_program(cmd, (int)(exit_code % 256));
	}
	else
		exit_program(cmd, 0);
	return (0);
}

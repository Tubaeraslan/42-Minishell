/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conditions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:01:27 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/29 13:03:24 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_heredoc_interrupt(t_command *command)
{
	command->last_exit_code = 130;
	g_signal_status = 0;
	if (command->tmp->input)
	{
		free(command->tmp->input);
		command->tmp->input = NULL;
	}
	command->is_heredoc = 0;
	command->heredoc_fd = -1;
}

int	check_heredoc_conditions(t_command *command)
{
	if (g_signal_status == 130)
	{
		handle_heredoc_interrupt(command);
		return (0);
	}
	if (has_any_heredoc(command) && command->heredoc_fd == -1)
	{
		handle_heredoc_interrupt(command);
		return (0);
	}
	return (1);
}

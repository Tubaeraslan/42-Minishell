/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_no_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:42:34 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/23 17:08:33 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_a_token(t_command *command)
{
	pid_t	pid;
	int		status;

	if (!command || !command->cmd)
		return ;
	if (command->is_heredoc)
		setup_heredoc(command);
	if (is_built(command->cmd) == 1)
	{
		execute_builtin_with_redir(command);
		return ;
	}
	pid = fork();
	if (pid == -1)
		handle_fork_error();
	else if (pid == 0)
	{
		handle_redirections(command);
		execute_child_process(command);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		command->last_exit_code = WEXITSTATUS(status);
	}
}

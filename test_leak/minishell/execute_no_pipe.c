/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_no_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:42:34 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/13 13:15:33 by teraslan         ###   ########.fr       */
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
	// signal(SIGINT, SIG_IGN);
	// signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		handle_fork_error();
	else if (pid == 0){
		// signal(SIGINT, SIG_DFL);
		// signal(SIGQUIT, SIG_DFL);
		execute_child_process(command);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		command->last_exit_code = WEXITSTATUS(status);
	}
}

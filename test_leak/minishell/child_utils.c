/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 12:51:33 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/19 13:03:51 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child(t_command *cmd, int prev_fd, int *fd)
{
	if (cmd->parsing_error)
		exit(1);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	if (handle_redirections(cmd) == -1)
		exit(1);
	if (is_built(cmd->cmd))
	{
		execute_built(cmd);
		exit(cmd->last_exit_code);
	}
	exec_external_or_exit(cmd);
}

pid_t	handle_fork(t_command *cmd, int prev_fd, int *fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		handle_child(cmd, prev_fd, fd);
	return (pid);
}

int	execute_pipeline(t_command *cmd, pid_t *pids)
{
	int		fd[2];
	int		prev_fd;
	int		i;
	pid_t	last_pid;

	prev_fd = -1;
	i = 0;
	last_pid = -1;
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			return (perror("pipe"), -1);
		pids[i++] = handle_fork(cmd, prev_fd, fd);
		last_pid = pids[i - 1];
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
			close(fd[1]);
		if (cmd->next)
			prev_fd = fd[0];
		else
			prev_fd = -1;
		cmd = cmd->next;
	}
	return (wait_for_children(pids, i, last_pid));
}

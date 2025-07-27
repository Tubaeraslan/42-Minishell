/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 12:51:33 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/27 16:07:26 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_stdout(t_command *cmd, int *fd)
{
	if (cmd->next)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 fd[1]");
			all_free(cmd);
			exit(1);
		}
		close(fd[1]);
	}
}

static void	handle_child(t_command *cmd, int prev_fd, int *fd)
{
	if (cmd->parsing_error)
	{
		all_free(cmd);
		exit(1);
	}
	setup_stdin(cmd, prev_fd);
	setup_stdout(cmd, fd);
	if (handle_redirections(cmd) == -1)
	{
		all_free(cmd);
		exit(1);
	}
	if (is_built(cmd->cmd))
	{
		execute_built(cmd);
		all_free(cmd);
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
		all_free(cmd);
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

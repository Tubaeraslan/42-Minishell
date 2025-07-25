/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:11:27 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/25 15:50:48 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_heredoc_setup(t_command *cmd)
{
	if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		exit(1);
	}
	close(cmd->heredoc_fd);
}

static void	handle_infile_setup(t_command *cmd)
{
	int	fd_in;

	if (access(cmd->infile, F_OK) != 0)
	{
		perror(cmd->infile);
		exit(1);
	}
	fd_in = open(cmd->infile, O_RDONLY);
	if (fd_in < 0)
	{
		perror(cmd->infile);
		exit(1);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2 infile");
		exit(1);
	}
	close(fd_in);
}

static void	handle_prev_fd(int prev_fd)
{
	if (dup2(prev_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 prev_fd");
		exit(1);
	}
	close(prev_fd);
}

void	setup_stdin(t_command *cmd, int prev_fd)
{
	if (cmd->is_heredoc)
		handle_heredoc_setup(cmd);
	else if (cmd->infile)
		handle_infile_setup(cmd);
	else if (prev_fd != -1)
		handle_prev_fd(prev_fd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:19:42 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/29 13:29:58 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parent_heredoc_process(t_command *cmd, int pipe_fd[2], pid_t pid)
{
	int	status;

	set_signal(1);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (cmd->heredoc_fd != -1)
			close(cmd->heredoc_fd);
		return (0);
	}
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	cmd->heredoc_fd = pipe_fd[0];
	cmd->is_heredoc = 1;
	return (1);
}

static void	child_heredoc_process(char *limiter, int pipe_fd[2])
{
	close(pipe_fd[0]);
	set_signal(0);
	heredoc_loop_custom(limiter, pipe_fd[1]);
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
}

static int	fork_and_handle_heredoc(t_command *cmd, char *limiter)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		child_heredoc_process(limiter, pipe_fd);
	return (parent_heredoc_process(cmd, pipe_fd, pid));
}

void	add_heredoc_node(t_heredoc **h, t_heredoc **l, char *input, int *i)
{
	t_heredoc	*node;

	node = malloc(sizeof(t_heredoc));
	if (!node)
		return ;
	node->limiter = extract_limiter(input, *i + 2);
	node->index = *i;
	node->next = NULL;
	if (!*h)
		*h = node;
	else
		(*l)->next = node;
	*l = node;
	*i += 2;
}

void	process_heredoc_list(t_command *cmd, t_heredoc *heredocs)
{
	t_heredoc	*tmp;

	tmp = heredocs;
	while (tmp)
	{
		if (!fork_and_handle_heredoc(cmd, tmp->limiter))
		{
			cmd->heredoc_fd = -1;
			cmd->is_heredoc = 0;
			g_signal_status = 130;
			return ;
		}
		tmp = tmp->next;
	}
}

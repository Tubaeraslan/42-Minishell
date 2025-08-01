/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:03:05 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/01 19:36:22 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_fds(t_command *cmd)
{
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	if (cmd->in_fd != -1)
		close(cmd->in_fd);
	if (cmd->out_fd != -1)
		close(cmd->out_fd);
}

void	free_heredoc(t_command *command)
{
	static t_command	*stored_command;
	t_heredoc			*tmp_heredoc;
	int					fd;

	stored_command = NULL;
	if (command)
	{
		stored_command = command;
		return ;
	}
	if (!stored_command)
		return ;
	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
	while (stored_command->heredocs)
	{
		tmp_heredoc = stored_command->heredocs;
		stored_command->heredocs = stored_command->heredocs->next;
		if (tmp_heredoc->limiter)
			free(tmp_heredoc->limiter);
		free(tmp_heredoc);
	}
	close_fds(stored_command);
	all_free(stored_command);
}

void	free_redirects(t_redirects *redir)
{
	t_redirects	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->name)
		{
			free(redir->name);
			redir->name = NULL;
		}
		free(redir);
		redir = tmp;
	}
}

void	free_heredocs(t_heredoc *heredoc)
{
	t_heredoc	*tmp;

	while (heredoc)
	{
		tmp = heredoc->next;
		if (heredoc->limiter)
		{
			free(heredoc->limiter);
			heredoc->limiter = NULL;
		}
		free(heredoc);
		heredoc = tmp;
	}
}

void	free_command_list_except_first(t_command *cmd)
{
	t_command	*tmp;
	t_command	*next_cmd;

	if (!cmd || !cmd->next)
		return ;
	tmp = cmd->next;
	while (tmp)
	{
		free_command_fields(tmp);
		next_cmd = tmp->next;
		free(tmp);
		tmp = next_cmd;
	}
	cmd->next = NULL;
}

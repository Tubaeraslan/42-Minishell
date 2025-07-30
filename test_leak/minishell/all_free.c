/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:07:03 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/30 13:51:24 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_command_strings(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->cmd)
	{
		free(cmd->cmd);
		cmd->cmd = NULL;
	}
	if (cmd->args)
	{
		free_two_dimension(cmd->args);
		cmd->args = NULL;
	}
}

static void	free_command_files(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->infile)
	{
		free(cmd->infile);
		cmd->infile = NULL;
	}
	if (cmd->outfile)
	{
		free(cmd->outfile);
		cmd->outfile = NULL;
	}
	if (cmd->heredoc_limiter)
	{
		free(cmd->heredoc_limiter);
		cmd->heredoc_limiter = NULL;
	}
}

static void	free_basic_fields(t_command *cmd)
{
	free_command_strings(cmd);
	free_command_files(cmd);
}

static void	free_struct_fields(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->tokens)
		clear_tokens(cmd);
	if (cmd->redirects)
	{
		free_redirects(cmd->redirects);
		cmd->redirects = NULL;
	}
	if (cmd->heredocs)
	{
		free_heredocs(cmd->heredocs);
		cmd->heredocs = NULL;
	}
}

void	free_command_fields(t_command *cmd)
{
	free_basic_fields(cmd);
	free_struct_fields(cmd);
}

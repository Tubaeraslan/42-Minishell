/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free_st.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:09:21 by ican              #+#    #+#             */
/*   Updated: 2025/08/02 17:57:23 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_two_dimension(char **arg)
{
	int	i;

	i = 0;
	if (!arg)
		return ;
	while (arg[i])
	{
		if (arg[i])
		{
			free(arg[i]);
			arg[i] = NULL;
		}
		i++;
	}
	free(arg);
	arg = NULL;
}

void	free_data(t_data *tmp)
{
	if (!tmp)
		return ;
	if (tmp->env)
	{
		free_two_dimension(tmp->env);
		tmp->env = NULL;
	}
	if (tmp->export_list)
	{
		free_two_dimension(tmp->export_list);
		tmp->export_list = NULL;
	}
	if (tmp->input)
	{
		free(tmp->input);
		tmp->input = NULL;
	}
	free(tmp);
}

void	all_free(t_command *cmd)
{
	if (!cmd)
		return ;
	if ((cmd->tokens && cmd->tokens[0]) || cmd->tokens != NULL)
	{
		clear_tokens(cmd);
		cmd->tokens = NULL;
	}
	if (cmd->cmd && ft_strlen(cmd->cmd) > 0)
	{
		free(cmd->cmd);
		cmd->cmd = NULL;
	}
	if (cmd->args && cmd->args[0])
	{
		free_two_dimension(cmd->args);
		cmd->args = NULL;
	}
	clear_command_data(cmd);
	free_data(cmd->tmp);
	cmd->tmp = NULL;
	free(cmd);
	cmd = NULL;
}

void	free_command_chain(t_command *cmd)
{
	t_command	*next;
	t_data		*tmp_data;

	if (!cmd)
		return ;
	tmp_data = cmd->tmp;
	if (cmd->pids)
	{
		free(cmd->pids);
		cmd->pids = NULL;
	}
	while (cmd)
	{
		next = cmd->next;
		if (cmd->heredoc_fd != -1)
		{
			close(cmd->heredoc_fd);
			cmd->heredoc_fd = -1;
		}
		if ((cmd->tokens && cmd->tokens[0]) || cmd->tokens != NULL)
		{
			clear_tokens(cmd);
			cmd->tokens = NULL;
		}
		if (cmd->cmd && ft_strlen(cmd->cmd) > 0)
		{
			free(cmd->cmd);
			cmd->cmd = NULL;
		}
		if (cmd->args && cmd->args[0])
		{
			free_two_dimension(cmd->args);
			cmd->args = NULL;
		}
		clear_command_data(cmd);
		cmd->tmp = NULL;
		free(cmd);
		cmd = next;
	}
	if (tmp_data)
		free_data(tmp_data);
}

void	clear_command_data(t_command *cmd)
{
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
	if (cmd->heredocs)
	{
		free_heredocs(cmd->heredocs);
		cmd->heredocs = NULL;
	}
	cmd->is_pipe = 0;
	cmd->is_append = 0;
	cmd->error_printed = 0;
	cmd->parsing_error = 0;
}

void	free_old_tokens(t_command *command)
{
	int	j;

	if (command->tokens)
	{
		j = 0;
		while (j < command->token_count)
		{
			free(command->tokens[j]);
			command->tokens[j] = NULL;
			j++;
		}
		free(command->tokens);
		command->tokens = NULL;
	}
}

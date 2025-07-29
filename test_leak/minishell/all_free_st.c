/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free_st.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:09:21 by ican              #+#    #+#             */
/*   Updated: 2025/07/29 17:26:43 by teraslan         ###   ########.fr       */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:42:35 by teraslan          #+#    #+#             */
/*   Updated: 2025/05/31 19:55:58 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void clear_command_data(t_command *cmd)
{
	if (cmd->cmd)
	{
		free(cmd->cmd);
		cmd->cmd = NULL;
	}
	if (cmd->args)
	{
		for (int i = 0; cmd->args[i]; i++)
			free(cmd->args[i]);
		free(cmd->args);
		cmd->args = NULL;
	}
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
	cmd->is_pipe = 0;
	cmd->is_append = 0;
	cmd->next = NULL; 
}

void parsing(t_command *command)
{
	int i;
	int count;
	char **tmp_args;
	char *tmp_token;
	int pipe_token_count;
	t_command *next_cmd;

	clear_command_data(command);
	pipe_token_count = 0;
	i = 0;
	count = 0;
	tmp_args = malloc(sizeof(char *) * (command->token_count + 1));
	if (!tmp_args)
		return;

	while (i < command->token_count)
	{
		tmp_token = command->tokens[i];
		if (ft_strncmp(tmp_token, "<", 2) == 0 && command->tokens[i + 1])
		{
			if (command->infile)
				free(command->infile);
			command->infile = ft_strdup(command->tokens[i + 1]);
			i += 2;
		}
		else if (ft_strncmp(tmp_token, ">>", 3) == 0 && command->tokens[i + 1])
		{
			if (command->outfile)
				free(command->outfile);
			command->outfile = ft_strdup(command->tokens[i + 1]);
			command->is_append = 1;
			i += 2;
		}
		else if (ft_strncmp(tmp_token, ">", 2) == 0 && command->tokens[i + 1])
		{
			if (command->outfile)
				free(command->outfile);
			command->outfile = ft_strdup(command->tokens[i + 1]);
			command->is_append = 0;
			i += 2;
		}
		else if (ft_strncmp(tmp_token, "|", 2) == 0)
		{
			command->is_pipe = 1;
			pipe_token_count = command->token_count - (i + 1);
			if (pipe_token_count > 0)
			{
				next_cmd = malloc(sizeof(t_command));
				if (!next_cmd)
					return;
				ft_memset(next_cmd, 0, sizeof(t_command));
				next_cmd->tokens = malloc(sizeof(char *) * pipe_token_count);
				if (!next_cmd->tokens)
				{
					free(next_cmd);
					return;
				}
				for (int j = 0; j < pipe_token_count; j++)
					next_cmd->tokens[j] = ft_strdup(command->tokens[i + 1 + j]);
				next_cmd->token_count = pipe_token_count;
				next_cmd->tmp = command->tmp;
				command->next = next_cmd;

				// pipe sonrası komutu parse et
				parsing(next_cmd);
				break;
			}
		}
		else
		{
			tmp_args[count++] = ft_strdup(tmp_token);
			i++;
		}
	}
	tmp_args[count] = NULL;

	// Daha önce varsa free et
	if (command->args)
	{
		for (int k = 0; command->args[k]; k++)
			free(command->args[k]);
		free(command->args);
	}
	command->args = tmp_args;

	if (command->cmd)
		free(command->cmd);
	if (count > 0)
		command->cmd = ft_strdup(tmp_args[0]);
	else
		command->cmd = NULL;

}

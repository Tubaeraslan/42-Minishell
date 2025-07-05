/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:19:32 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/05 12:56:25 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void clear_command_data(t_command *cmd)
{
	if (cmd->cmd)
	{
		free(cmd->cmd);
		cmd->cmd = NULL;
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
	if (cmd->heredoc_limiter)
    {
        free(cmd->heredoc_limiter);
        cmd->heredoc_limiter = NULL;
    }
	cmd->is_pipe = 0;
	cmd->heredoc_fd = 0;
	cmd->is_heredoc = 0;
	cmd->is_append = 0;
	cmd->next = NULL; 
}

static int add_arg(char **args, int index, char *token)
{
	args[index] = ft_strdup(token);
	if (!args[index])
		return 0;
	return 1;
}

static char **alloc_args(int max_count)
{
	char **args = malloc(sizeof(char *) * (max_count + 1));
	if (!args)
		return NULL;
	for (int i = 0; i <= max_count; i++)
		args[i] = NULL;
	return args;
}

static int check_and_handle_redirect(t_command *command, char **tokens, int *i)
{
	char *tmp_token = tokens[*i];

	if (ft_strncmp(tmp_token, "<", 2) == 0 && tokens[*i + 1])
		return handle_infile(command, tokens, i);

	if (ft_strncmp(tmp_token, ">>", 3) == 0 && tokens[*i + 1])
		return handle_outfile(command, tokens, i, 1);

	if (ft_strncmp(tmp_token, ">", 2) == 0 && tokens[*i + 1])
		return handle_outfile(command, tokens, i, 0);

	if (ft_strncmp(tmp_token, "<<", 3) == 0 && tokens[*i + 1])
		return handle_heredoc(command, tokens, i);

	return 0;
}

void parsing(t_command *command)
{
	int i = 0;
	int count = 0;
	char **tmp_args;

	clear_command_data(command);
	tmp_args = alloc_args(command->token_count);
	if (!tmp_args)
		return;
	while (i < command->token_count)
	{
		char *tmp_token = command->tokens[i];
		if (check_and_handle_redirect(command, command->tokens, &i))
			continue;
		if (ft_strncmp(tmp_token, "|", 2) == 0)
		{
			create_next_command(command, command->tokens, i);
			break;
		}
		if (!add_arg(tmp_args, count, tmp_token))
			break;
		count++;
		i++;
	}
	tmp_args[count] = NULL;
	update_args_and_cmd(command, tmp_args, count);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:26:09 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/07 15:39:30 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_infile(t_command *command, char **tokens, int *i)
{
	char *filename = tokens[*i + 1];
	if (access(filename, R_OK) != 0)
	{
		command->last_exit_code = 1;
		return -1;
	}
	if (command->infile)
		free(command->infile);
	command->infile = ft_strdup(tokens[*i + 1]);
	*i += 2;
	return 1;
}

int handle_outfile(t_command *command, char **tokens, int *i, int append)
{
	char *filename = tokens[*i + 1];

	if (access(filename, F_OK) == 0 && access(filename, W_OK) != 0)
	{
		command->last_exit_code = 1;
		return -1;
	}
	if (command->outfile)
		free(command->outfile);
	command->outfile = ft_strdup(tokens[*i + 1]);
	command->is_append = append;
	*i += 2;
	return 1;
}

int handle_heredoc(t_command *command, char **tokens, int *i)
{
	if (command->heredoc_limiter)
		free(command->heredoc_limiter);
	command->heredoc_limiter = ft_strdup(tokens[*i + 1]);
	if (command->infile)
	{
		free(command->infile);
		command->infile = NULL;
	}
	command->is_heredoc = 1;
	*i += 2;
	return 1;
}

t_command *create_next_command(t_command *command, char **tokens, int index)
{
	int pipe_token_count = command->token_count - (index + 1);
	if (pipe_token_count <= 0)
		return NULL;

	t_command *next_cmd = malloc(sizeof(t_command));
	if (!next_cmd)
		return NULL;
	ft_memset(next_cmd, 0, sizeof(t_command));

	next_cmd->tokens = malloc(sizeof(char *) * pipe_token_count);
	if (!next_cmd->tokens)
	{
		free(next_cmd);
		return NULL;
	}
	for (int j = 0; j < pipe_token_count; j++)
		next_cmd->tokens[j] = ft_strdup(tokens[index + 1 + j]);
	next_cmd->token_count = pipe_token_count;
	next_cmd->tmp = command->tmp;
	command->next = next_cmd;
	command->is_pipe = 1;

	parsing(next_cmd);

	return next_cmd;
}

void update_args_and_cmd(t_command *command, char **args, int count)
{
    // Öncekini biz serbest bırakmıyoruz, dışarıda yapılmalı.
    command->args = args;

    if (command->cmd)
    {
        free(command->cmd);
        command->cmd = NULL;
    }

    if (count > 0 && args && args[0])
        command->cmd = ft_strdup(args[0]);
    else
        command->cmd = NULL;
}
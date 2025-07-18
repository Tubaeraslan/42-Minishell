/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:42:35 by teraslan          #+#    #+#             */
/*   Updated: 2025/06/30 20:23:22 by teraslan         ###   ########.fr       */
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

void clear_command(t_command *command)
{
	clear_command_data(command);
	if (command->heredoc_limiter)
	{
		free(command->heredoc_limiter);
		command->heredoc_limiter = NULL;
	}
}

int handle_infile(t_command *command, char **tokens, int *i)
{
	if (command->infile)
		free(command->infile);
	command->infile = ft_strdup(tokens[*i + 1]);
	*i += 2;
	return 1;
}

int handle_outfile(t_command *command, char **tokens, int *i, int append)
{
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

char **alloc_args(int max_count)
{
	char **args = malloc(sizeof(char *) * (max_count + 1));
	if (!args)
		return NULL;
	for (int i = 0; i <= max_count; i++)
		args[i] = NULL;
	return args;
}

int add_arg(char **args, int index, char *token)
{
	args[index] = ft_strdup(token);
	if (!args[index])
		return 0;
	return 1;
}

void update_args_and_cmd(t_command *command, char **args, int count)
{
	if (command->args)
	{
		for (int k = 0; command->args[k]; k++)
			free(command->args[k]);
		free(command->args);
	}
	command->args = args;

	if (command->cmd)
		free(command->cmd);
	if (count > 0)
		command->cmd = ft_strdup(args[0]);
	else
		command->cmd = NULL;
}

int check_and_handle_redirect(t_command *command, char **tokens, int *i)
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

	clear_command(command);
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

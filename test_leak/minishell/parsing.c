/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:19:32 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/19 13:47:42 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void clear_command_data(t_command *cmd)
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
	//free_data(cmd->tmp);
	cmd->is_pipe = 0;
	cmd->heredoc_fd = 0;
	cmd->is_heredoc = 0;
	cmd->is_append = 0;
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

	command->error_printed = 0;
	command->parsing_error = 0;
	clear_command_data(command);
	tmp_args = alloc_args(command->token_count);
	if (!tmp_args)
	{
		command->last_exit_code = 1;
		return;
	}
	while (i < command->token_count)
	{
		char *tmp_token = command->tokens[i];
		int redirect_result = check_and_handle_redirect(command, command->tokens, &i);
		if (redirect_result == 1)
			continue;
		else if (redirect_result == -1){
			command->parsing_error = 1;
			command->last_exit_code = 1;
			i++;
			continue;
		}
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

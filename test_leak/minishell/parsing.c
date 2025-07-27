/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:19:32 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/27 17:11:22 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_arg(char **args, int index, char *token)
{
	args[index] = ft_strdup(token);
	if (!args[index])
		return (0);
	return (1);
}

static char	**alloc_args(int max_count)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * (max_count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i <= max_count)
		args[i++] = NULL;
	return (args);
}

static int	check_and_handle_redirect(t_command *command, char **tokens, int *i)
{
	char	*tmp_token;

	tmp_token = tokens[*i];
	if (ft_strncmp(tmp_token, "<", 2) == 0 && tokens[*i + 1])
		return (handle_infile(command, tokens, i));
	if (ft_strncmp(tmp_token, ">>", 3) == 0 && tokens[*i + 1])
		return (handle_outfile(command, tokens, i, 1));
	if (ft_strncmp(tmp_token, ">", 2) == 0 && tokens[*i + 1])
		return (handle_outfile(command, tokens, i, 0));
	if (ft_strncmp(tmp_token, "<<", 3) == 0 && tokens[*i + 1])
		return (handle_heredoc(command, tokens, i));
	return (0);
}

static int	handle_tokens(t_command *cmd, char **tmp_args, int *count, int *i)
{
	char	*token;
	int		redirect_result;

	token = cmd->tokens[*i];
	redirect_result = check_and_handle_redirect(cmd, cmd->tokens, i);
	if (redirect_result == 1)
		return (1);
	else if (redirect_result == -1)
	{
		cmd->parsing_error = 1;
		cmd->last_exit_code = 1;
		(*i)++;
		return (1);
	}
	if (ft_strncmp(token, "|", 2) == 0)
	{
		create_next_command(cmd, cmd->tokens, *i);
		return (2);
	}
	if (!add_arg(tmp_args, *count, token))
		return (-1);
	(*count)++;
	(*i)++;
	return (0);
}

void	parsing(t_command *command)
{
	int		i;
	int		count;
	char	**tmp_args;
	int		result;

	i = 0;
	count = 0;
	clear_command_data(command);
	tmp_args = alloc_args(command->token_count);
	if (!tmp_args)
	{
		command->last_exit_code = 1;
		return ;
	}
	while (i < command->token_count)
	{
		result = handle_tokens(command, tmp_args, &count, &i);
		if (result == 1)
			continue ;
		if (result == 2 || result == -1)
			break ;
	}
	tmp_args[count] = NULL;
	update_args_and_cmd(command, tmp_args, count);
}

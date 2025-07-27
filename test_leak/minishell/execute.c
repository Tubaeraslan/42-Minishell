/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:34:23 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/27 15:12:40 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_executable(char *path, struct stat *st)
{
	if (stat(path, st) == -1)
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(path);
		exit(127);
	}
	if (S_ISDIR(st->st_mode))
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(path);
		exit(126);
	}
	if (!(st->st_mode & S_IXUSR))
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		free(path);
		exit(126);
	}
}

void	exec_command(t_command *command, char *path)
{
	execve(path, command->args, command->tmp->env);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": execve error\n", 2);
	free(path);
	exit(126);
}

void	handle_fork_error(t_command *command)
{
	perror("fork");
	all_free(command);
	exit(EXIT_FAILURE);
}

void	reset_flags(t_command *command)
{
	while (command)
	{
		command->is_pipe = 0;
		command->is_heredoc = 0;
		if (command->heredoc_limiter)
		{
			free(command->heredoc_limiter);
			command->heredoc_limiter = NULL;
		}
		if (command->heredoc_fd > 2)
		{
			close(command->heredoc_fd);
			command->heredoc_fd = -1;
		}
		command = command->next;
	}
}

void	execute_commands(t_command *command)
{
	if (!command || command->token_count == 0)
		return ;
	if (!command->is_pipe)
	{
		if (!command->parsing_error)
			execute_a_token(command);
	}
	else
	{
		command->last_exit_code = execute_many_token(command);
	}
	reset_flags(command);
	free_command_fields(command);
	
}

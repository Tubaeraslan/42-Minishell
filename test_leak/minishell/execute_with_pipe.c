/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_with_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:36:30 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/30 18:08:56 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_execute_command(t_command *cmd)
{
	char	*path;

	if (is_built(cmd->cmd))
	{
		execute_built(cmd);
		all_free(cmd);
		exit(0);
	}
	path = path_finder(cmd->cmd, cmd->tmp->env);
	if (!path)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putstr_fd(cmd->cmd, 2);
		ft_putchar_fd('\n', 2);
		free(path);
		all_free(cmd);
		exit(127);
	}
	execve(path, cmd->args, cmd->tmp->env);
	perror("execve");
	free(path);
	all_free(cmd);
	exit(EXIT_FAILURE);
}

static void	execute_commands_pipe(char *path, t_command *cmd)
{
	execve(path, cmd->args, cmd->tmp->env);
	perror("execve");
	free(path);
	all_free(cmd);
	exit(126);
}

void	exec_external_or_exit(t_command *cmd)
{
	char	*path;

	path = get_command_path(cmd);
	check_path_validity(path, cmd);
	execute_commands_pipe(path, cmd);
}

int	wait_for_children(pid_t *pids, int count, pid_t last_pid)
{
	int		status;
	int		exit_code;
	int		waited;
	pid_t	pid;

	exit_code = 0;
	waited = 0;
	(void)pids;
	while (waited < count)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == last_pid)
			exit_code = get_exit_code_from_status(status);
		waited++;
	}
	return (exit_code);
}

int	execute_many_token(t_command *command)
{
	int	cmd_count;
	int	exit_code;
	int	validation_result;

	validation_result = validate_all_commands(command);
	if (validation_result != 0)
		return (validation_result);
	cmd_count = count_commands(command);
	command->pids = malloc(sizeof(pid_t) * cmd_count);
	if (!command->pids)
	{
		all_free(command);
		return (perror("malloc"), EXIT_FAILURE);
	}
	exit_code = execute_pipeline(command, command->pids);
	free(command->pids);
	command->pids = NULL;

	if (exit_code == -1)
		return (EXIT_FAILURE);
	else
		return (exit_code);
}

int	validate_all_commands(t_command *command)
{
	t_command	*current;
	char		*path;

	current = command;
	while (current)
	{
		if (!is_built(current->cmd))
		{
			path = path_finder(current->cmd, current->tmp->env);
			if (!path)
			{
				ft_putstr_fd("command not found: ", 2);
				ft_putstr_fd(current->cmd, 2);
				ft_putchar_fd('\n', 2);
				return (127);
			}
			free(path);
		}
		current = current->next;
	}
	return (0);
}

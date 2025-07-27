/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_with_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:36:30 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/27 18:59:23 by ican             ###   ########.fr       */
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
		all_free(cmd);
		exit(127);
	}
	execve(path, cmd->args, cmd->tmp->env);
	perror("execve");
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
	check_path_validity(path);
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
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
		}
		waited++;
	}
	return (exit_code);
}

int	execute_many_token(t_command *command)
{
	t_command	*cmd;
	int			cmd_count;
	pid_t		*pids;
	int			exit_code;

	cmd = command;
	cmd_count = count_commands(cmd);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		all_free(command);
		return (perror("malloc"), EXIT_FAILURE);
	}
	exit_code = execute_pipeline(cmd, pids);
	free(pids);
	if (exit_code == -1)
		return (EXIT_FAILURE);
	else
		return (exit_code);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_with_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:36:30 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/19 13:04:46 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_execute_command(t_command *cmd)
{
	char	*path;

	if (is_built(cmd->cmd))
	{
		execute_built(cmd);
		exit(0);
	}
	path = path_finder(cmd->cmd, cmd->tmp->env);
	if (!path)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putstr_fd(cmd->cmd, 2);
		ft_putchar_fd('\n', 2);
		exit(127);
	}
	execve(path, cmd->args, cmd->tmp->env);
	perror("execve");
	exit(EXIT_FAILURE);
}

static int	count_commands(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	exec_external_or_exit(t_command *cmd)
{
	char	*path;

	path = path_finder(cmd->cmd, cmd->tmp->env);
	if (!path)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putstr_fd(cmd->cmd, 2);
		ft_putchar_fd('\n', 2);
		exit(127);
	}
	execve(path, cmd->args, cmd->tmp->env);
	perror("execve");
	exit(EXIT_FAILURE);
}

int	wait_for_children(pid_t *pids, int count, pid_t last_pid)
{
	int	status;
	int	exit_code;
	int	j;

	exit_code = 0;
	j = 0;
	while (j < count)
	{
		waitpid(pids[j], &status, 0);
		if (pids[j] == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
			else
				exit_code = 1;
		}
		j++;
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
		return (perror("malloc"), EXIT_FAILURE);
	exit_code = execute_pipeline(cmd, pids);
	free(pids);
	if (exit_code == -1)
		return (EXIT_FAILURE);
	else
		return (exit_code);
}

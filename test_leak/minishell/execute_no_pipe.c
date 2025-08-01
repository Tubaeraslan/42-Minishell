/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_no_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:42:34 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/01 19:16:17 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_child_process(t_command *command)
{
	struct stat	st;
	char		*path;

	if (command->cmd[0] == '/'
		|| (command->cmd[0] == '.' && command->cmd[1] == '/'))
		path = ft_strdup(command->cmd);
	else
		path = path_finder(command->cmd, command->tmp->env);
	if (!path)
	{
		ft_putstr_fd(command->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		free(path);
		all_free(command);
		exit(127);
	}
	check_executable(path, &st, command);
	exec_command(command, path);
}

static void	run_child_process(t_command *command)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_redirections(command);
	execute_child_process(command);
	all_free(command);
	exit(EXIT_FAILURE);
}

static void	set_exit_code_from_status(t_command *command, int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(1, "\r", 1);
			command->last_exit_code = 130;
		}
		else if (sig == SIGQUIT)
		{
			write(1, "Quit (core dumped)\n", 19);
			command->last_exit_code = 131;
		}
		else
			command->last_exit_code = 128 + sig;
	}
	else if (WIFEXITED(status))
		command->last_exit_code = WEXITSTATUS(status);
}

void	execute_a_token(t_command *command)
{
	pid_t	pid;
	int		status;

	if (!command || !command->cmd)
		return ;
	if (is_built(command->cmd))
	{
		execute_builtin_with_redir(command);
		return ;
	}
	pid = fork();
	if (pid == -1)
		handle_fork_error(command);
	else if (pid == 0)
		run_child_process(command);
	else
	{
		waitpid(pid, &status, 0);
		set_exit_code_from_status(command, status);
	}
}

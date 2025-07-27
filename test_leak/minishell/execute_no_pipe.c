/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_no_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:42:34 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/27 18:24:36 by teraslan         ###   ########.fr       */
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
		exit(127);
	}
	check_executable(path, &st);
	exec_command(command, path);
}

void	execute_a_token(t_command *command)
{
	pid_t	pid;
	int		status;

	if (!command || !command->cmd)
		return ;
	if (is_built(command->cmd) == 1)
	{
		execute_builtin_with_redir(command);
		return ;
	}
	pid = fork();
	if (pid == -1)
		handle_fork_error(command);
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL); // Child sinyali alsın
		signal(SIGQUIT, SIG_DFL); 
		handle_redirections(command);
		execute_child_process(command);
		all_free(command);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			int	sig = WTERMSIG(status);
			if (sig == SIGINT)
			{
				write(1, "\r", 1);
				command->last_exit_code = 130;
			}
			else if (sig == SIGQUIT)
			{
				write(1, "Quit (core dumped)\n", 19);
				command->last_exit_code = 131; // SIGQUIT çıkış kodu genelde 131'dir
			}
			else
				command->last_exit_code = 128 + sig;
		}
		else if (WIFEXITED(status))
			command->last_exit_code = WEXITSTATUS(status);
	}
}

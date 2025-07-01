/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_with_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:36:30 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/01 18:47:42 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_pipe(int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

static void	parent_process_logic(t_command *command, int pre_fd, int fd[2], pid_t pid)
{
	if (pre_fd != -1)
		close(pre_fd);
	if (command->next)
		close(fd[1]);
	else
		close(fd[0]);
	waitpid(pid, NULL, 0);
}

void	child_execute_command(t_command *cmd)
{
	char *path;

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

static void	child_process_logic(t_command *cmd, int pre_fd, int fd[2])
{
	if (pre_fd != -1)
	{
		dup2(pre_fd, STDIN_FILENO);
		close(pre_fd);
	}
	if (cmd->next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	handle_redirections(cmd);
	child_execute_command(cmd);
}

static void	handle_fork_process(pid_t pid, t_command *command, int pre_fd, int fd[2])
{
	if (pid == 0)
		child_process_logic(command, pre_fd, fd);
	else
		parent_process_logic(command, pre_fd, fd, pid);
}

void	execute_many_token(t_command *command)
{
	int		fd[2];
	int		pre_fd;
	pid_t	pid;

	pre_fd = -1;
	while (command)
	{
		if (command->next && init_pipe(fd) == -1)
			exit(EXIT_FAILURE);
		if (command->is_heredoc)
			setup_heredoc(command);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		handle_fork_process(pid, command, pre_fd, fd);
		if (command->next)
			pre_fd = fd[0];
		command = command->next;
	}
}

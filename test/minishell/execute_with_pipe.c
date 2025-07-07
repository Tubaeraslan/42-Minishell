/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_with_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:36:30 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/07 18:02:31 by teraslan         ###   ########.fr       */
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

void execute_many_token(t_command *command)
{
    int fd[2];
    int pre_fd = -1;
    pid_t pid;
    t_command *cmd = command;
    pid_t *pids;
    int count = 0;
    int i = 0;
    int status;
    int last_exit_status = 0;

    // Kaç komut var say
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    pids = malloc(sizeof(pid_t) * count);
    if (!pids)
        exit(EXIT_FAILURE);

    pre_fd = -1;
    cmd = command;
    i = 0;
    while (cmd)
    {
        if (cmd->next && init_pipe(fd) == -1)
            exit(EXIT_FAILURE);
        if (cmd->is_heredoc)
            setup_heredoc(cmd);

        if (cmd->parsing_error)
        {
            pid = fork();
            if (pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (pid == 0)
                exit(1); // çocuk process'te çık
            pids[i++] = pid; // parent process'te PID'yi kaydet
            cmd = cmd->next;
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            if (pre_fd != -1)
            {
                dup2(pre_fd, STDIN_FILENO);
                close(pre_fd);
            }
            handle_redirections(cmd);
            if (cmd->next)
            {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }
            if (is_built(cmd->cmd))
            {
                execute_built(cmd);
                exit(cmd->last_exit_code);
            }
            else
            {
                char *path = path_finder(cmd->cmd, cmd->tmp->env);
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
        }
        else
        {
            if (pre_fd != -1)
                close(pre_fd);
            if (cmd->next)
                close(fd[1]);
            pre_fd = (cmd->next) ? fd[0] : -1;
            pids[i++] = pid;
            cmd = cmd->next;
        }
    }

    // Tüm çocukları bekle ve sonuncusunun exit statusunu al
    for (int j = 0; j < count; j++)
    {
        waitpid(pids[j], &status, 0);
        if (j == count - 1)
        {
            if (WIFEXITED(status))
                last_exit_status = WEXITSTATUS(status);
            else if(WIFSIGNALED(status))
                last_exit_status = 128 + WTERMSIG(status);
            else
                last_exit_status = 1; // abnormal exit
        }
    }
    free(pids);

    command->last_exit_code = last_exit_status;
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_with_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:36:30 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/15 18:11:47 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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

int execute_many_token(t_command *command)
{
    t_command *cmd = command;
    int fd[2];
    int prev_fd = -1;
    pid_t *pids;
    int cmd_count = 0;
    pid_t last_pid = -1;

    while (cmd)
    {
        cmd_count++;
        cmd = cmd->next;
    }

    pids = malloc(sizeof(pid_t) * cmd_count);
    if (!pids)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    cmd = command;
    int i = 0;
    while (cmd)
    {
        if (cmd->next && pipe(fd) == -1)
        {
            perror("pipe");
            free(pids);
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(pids);
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            // Çocuk süreç

            // Parsing error varsa exit 1 yap ve çık
            if (cmd->parsing_error)
                exit(1);

            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (cmd->next)
            {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }

            if (handle_redirections(cmd) == -1)
                exit(1);

            if (is_built(cmd->cmd))
            {
                execute_built(cmd);
                exit(cmd->last_exit_code);
            }

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
        else
        {
            // Parent process
            pids[i++] = pid;
            last_pid = pid;

            if (prev_fd != -1)
                close(prev_fd);
            if (cmd->next)
                close(fd[1]);

            prev_fd = (cmd->next) ? fd[0] : -1;
            cmd = cmd->next;
        }
    }

    int status;
    int exit_code = 0;
    for (int j = 0; j < i; j++)
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
    }

    free(pids);
    return exit_code;
}






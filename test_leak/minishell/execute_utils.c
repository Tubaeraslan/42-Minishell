/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:41:22 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/24 13:56:40 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void setup_heredoc(t_command *cmd)
{
    int pipe_fd[2];
    char *line;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        cmd->heredoc_fd = -1;
        exit(EXIT_FAILURE);
    }

    // Eğer pipe_fd[0] 0,1 veya 2 ise dup kullanarak yeni fd al, eskiyi kapat
    if (pipe_fd[0] <= 2)
    {
        int new_fd = dup(pipe_fd[0]);
        close(pipe_fd[0]);
        pipe_fd[0] = new_fd;
    }

    // Aynı şekilde pipe_fd[1] için de
    if (pipe_fd[1] <= 2)
    {
        int new_fd = dup(pipe_fd[1]);
        close(pipe_fd[1]);
        pipe_fd[1] = new_fd;
    }

    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strncmp(line, cmd->heredoc_limiter, ft_strlen(cmd->heredoc_limiter) + 1) == 0)
        {
            free(line);
            break;
        }
        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    cmd->heredoc_fd = pipe_fd[0];
}

static int	handle_input_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->is_heredoc)
	{
		if (cmd->heredoc_fd < 0)
			return (-1);
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
			return (-1);
		close(cmd->heredoc_fd);
		return (0);
	}
	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
		{
			perror(cmd->infile);
			return (-1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2 infile");
			close(fd);
			return (-1);
		}
		close(fd);
	}
	return (0);
}

static int	handle_output_redirection(t_command *cmd)
{
	int	fd;

	if (!cmd->outfile)
		return (0);
	if (cmd->is_append)
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(cmd->outfile);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirections(t_command *cmd)
{
	if (handle_input_redirection(cmd) < 0)
		return (-1);
	if (handle_output_redirection(cmd) < 0)
		return (-1);
	return (0);
}

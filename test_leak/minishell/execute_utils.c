/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:41:22 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/24 18:26:31 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void strip_newline(char *str)
{
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
    {
        str[len - 1] = '\0';
        len--;
    }
}

void setup_heredoc(t_command *cmd)
{
    int pipe_fd[2];
    char *line;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        // Satır sonunu temizleme fonksiyonu uygunsa çağır (strip_newline gibi)

        if (strcmp(line, cmd->heredoc_limiter) == 0)
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
    cmd->is_heredoc = 1;
}

static int	handle_input_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->is_heredoc)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
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
		dup2(fd, STDIN_FILENO);
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

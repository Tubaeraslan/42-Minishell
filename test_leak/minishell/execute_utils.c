/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:41:22 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/30 12:45:05 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	strip_newline(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
	{
		str[len - 1] = '\0';
		len--;
	}
}

static int	handle_input_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->is_heredoc)
	{
		cmd->in_fd = cmd->heredoc_fd;
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		//close(cmd->heredoc_fd);
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
		cmd->in_fd = fd;
		dup2(fd, STDIN_FILENO);
		//close(fd);
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
	cmd->out_fd = fd;
	dup2(fd, STDOUT_FILENO);
	//close(fd);
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

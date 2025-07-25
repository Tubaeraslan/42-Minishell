/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:20:14 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/25 16:57:36 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_loop(t_command *cmd, int pipe_write_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (0);
		if (ft_strncmp(line, cmd->heredoc_limiter,
				ft_strlen(cmd->heredoc_limiter) + 1) == 0)
		{
			free(line);
			return (0);
		}
		write(pipe_write_fd, line, ft_strlen(line));
		write(pipe_write_fd, "\n", 1);
		free(line);
	}
	return (1);
}

char	*extract_limiter(char *input, int i)
{
	int		start;
	int		len;
	char	*limiter;

	while (input[i] == ' ' || input[i] == '\t')
		i++;
	start = i;
	while (input[i] && input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
		i++;
	len = i - start;
	if (len == 0)
		return (NULL);
	limiter = malloc(len + 1);
	if (!limiter)
		return (NULL);
	ft_strncpy(limiter, input + start, len);
	limiter[len] = '\0';
	return (limiter);
}

char	*get_heredoc_limiter(char *input)
{
	int		i;
	int		inside_squote;
	int		inside_dquote;

	i = 0;
	inside_squote = 0;
	inside_dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !inside_dquote)
			inside_squote = !inside_squote;
		else if (input[i] == '\"' && !inside_squote)
			inside_dquote = !inside_dquote;
		if (!inside_squote && !inside_dquote)
		{
			if (input[i] == '<' && input[i + 1] == '<')
				return (extract_limiter(input, i + 2));
		}
		i++;
	}
	return (NULL);
}

int	has_heredoc(char *input)
{
	return (ft_strnstr(input, "<<", ft_strlen(input)) != NULL);
}

void	check_heredoc_and_setup(t_command *command)
{
	t_command	*cmd;

	cmd = command;
	while (cmd)
	{
		if (has_heredoc(cmd->tmp->input) && cmd->heredoc_fd == -1)
		{
			cmd->heredoc_limiter = get_heredoc_limiter(cmd->tmp->input);
			if (cmd->heredoc_limiter)
				setup_heredoc(cmd);
		}
		cmd = cmd->next;
	}
}

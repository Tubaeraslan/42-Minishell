/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read_input.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:12:17 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/01 19:23:55 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredoc_list(t_heredoc *lst)
{
	t_heredoc	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->limiter);
		free(lst);
		lst = tmp;
	}
}

static int	handle_heredoc_line(char *line, char *limiter, int write_fd)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
	{
		free(line);
		return (0);
	}
	write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
	free(line);
	return (1);
}

static int	read_chars_to_buffer(char *buffer, int max_len)
{
	int		i;
	char	c;

	i = 0;
	while (i < max_len - 1)
	{
		if (read(0, &c, 1) <= 0)
			break ;
		if (c == '\n')
		{
			buffer[i++] = c;
			break ;
		}
		buffer[i++] = c;
		if (g_signal_status == 130)
			break ;
	}
	buffer[i] = '\0';
	return (i);
}

static char	*read_heredoc_line(void)
{
	char	buffer[1024];
	char	*line;
	int		len;

	len = read_chars_to_buffer(buffer, 1024);
	if (len == 0)
		return (NULL);
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	ft_strlcpy(line, buffer, len + 1);
	return (line);
}

void	heredoc_loop_custom(char *limiter, int write_fd)
{
	char	*line;

	while (1)
	{
		if (g_signal_status == 130)
			break ;
		write(1, "> ", 2);
		line = read_heredoc_line();
		if (!line || g_signal_status == 130)
		{
			free(line);
			break ;
		}
		if (!handle_heredoc_line(line, limiter, write_fd))
			break ;
	}
}

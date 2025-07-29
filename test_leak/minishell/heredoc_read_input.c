/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read_input.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:12:17 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/29 19:05:19 by teraslan         ###   ########.fr       */
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

void	heredoc_loop_custom(char *limiter, int write_fd)
{
	char	*line;

	while (1)
	{
		if (g_signal_status == 130)
			break ;
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line || g_signal_status == 130)
		{
			free(line);
			break ;
		}
		if (!handle_heredoc_line(line, limiter, write_fd))
			break ;
	}
}

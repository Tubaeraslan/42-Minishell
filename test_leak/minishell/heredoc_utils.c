/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:20:14 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/26 19:30:55 by ican             ###   ########.fr       */
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
t_heredoc	*get_all_heredocs(char *input)
{
	t_heredoc	*head = NULL;
	t_heredoc	*last = NULL;
	int			i = 0;
	int			in_squote = 0, in_dquote = 0;

	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (input[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		if (!in_squote && !in_dquote && input[i] == '<' && input[i + 1] == '<')
		{
			t_heredoc *node = malloc(sizeof(t_heredoc));
			node->limiter = extract_limiter(input, i + 2); // aynı fonksiyonu kullan
			node->index = i;
			node->next = NULL;
			if (!head)
				head = node;
			else
				last->next = node;
			last = node;
			i += 2;
		}
		else
			i++;
	}
	return head;
}

void	free_heredoc_list(t_heredoc *lst)
{
	t_heredoc *tmp;
	while (lst)
	{
		tmp = lst->next;
		free(lst->limiter);
		free(lst);
		lst = tmp;
	}
}

void	heredoc_loop_custom(char *limiter, int write_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			free(line);
			break;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
}

void	check_heredoc_and_setup(t_command *command)
{
	t_command	*cmd = command;
	t_heredoc	*heredocs;
	t_heredoc	*tmp;

	while (cmd)
	{
		if (has_heredoc(cmd->tmp->input) && cmd->heredoc_fd == -1)
		{
			heredocs = get_all_heredocs(cmd->tmp->input);
			tmp = heredocs;
			while (tmp)
			{
				int pipe_fd[2];
				if (pipe(pipe_fd) == -1)
				{
					free_heredoc_list(heredocs);
					all_free(command);
					perror("pipe");
					exit(EXIT_FAILURE);
				}
				// Her heredoc için ayrı ayrı input al
				heredoc_loop_custom(tmp->limiter, pipe_fd[1]);
				close(pipe_fd[1]);
				cmd->heredoc_fd = pipe_fd[0];
				cmd->is_heredoc = 1;
				tmp = tmp->next;
			}
			// Heredoc listesi temizlenmeli
			free_heredoc_list(heredocs);
		}
		cmd = cmd->next;
	}
}

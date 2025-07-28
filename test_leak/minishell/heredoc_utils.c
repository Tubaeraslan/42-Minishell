/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:20:14 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/28 13:38:39 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int has_heredoc(char *input)
{
    int i = 0;
    int inside_squote = 0;
    int inside_dquote = 0;

    while (input[i])
    {
        if (input[i] == '\'' && !inside_dquote)
            inside_squote = !inside_squote;
        else if (input[i] == '\"' && !inside_squote)
            inside_dquote = !inside_dquote;
        else if (!inside_squote && !inside_dquote)
        {
            if (input[i] == '<' && input[i + 1] == '<')
                return 1;
        }
        i++;
    }
    return 0;
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
	size_t	len;

	while (1)
	{
		if (g_signal_status == 130)
			break;

		write(1, "> ", 2); // prompt elle veriliyor
		line = get_next_line(0);
		if (!line || g_signal_status == 130)
		{
			free(line);
			break;
		}

		// Satır sonundaki newline'i temizle
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';

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

void	child_signal_handler2(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_status = 130;
		signal(SIGINT, SIG_IGN);
	}
}

void	child_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_status = 130;
		printf("\n");
		exit(g_signal_status);
	}
}

void set_signal(int i)
{
	if (i == 0)
	{
		signal(SIGINT, child_signal_handler);
	}
	if (i == 1)
	{
		signal(SIGINT, child_signal_handler2);
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
				pid_t pid;

				if (pipe(pipe_fd) == -1)
				{
					perror("pipe");
					exit(EXIT_FAILURE);
				}

				pid = fork();
				if (pid < 0)
				{
					perror("fork");
					exit(EXIT_FAILURE);
				}
				else if (pid == 0)
				{
					// Child process
					close(pipe_fd[0]);
					set_signal(0);
					heredoc_loop_custom(tmp->limiter, pipe_fd[1]);
					close(pipe_fd[1]);
					exit(EXIT_SUCCESS);
				}
				else
				{
					// Parent process
					set_signal(1);
					close(pipe_fd[1]);

					int status;
					waitpid(pid, &status, 0);

					if (WIFSIGNALED(status))
					{
						// Eğer daha önce açık heredoc_fd varsa kapat
						if (cmd->heredoc_fd != -1)
							close(cmd->heredoc_fd);
						cmd->heredoc_fd = -1;
						cmd->is_heredoc = 0;
						g_signal_status = 130;
						free_heredoc_list(heredocs);
						return;
					}

					// Önceden açık heredoc_fd varsa kapat (fd sızıntısı önleme)
					if (cmd->heredoc_fd != -1)
						close(cmd->heredoc_fd);
					cmd->heredoc_fd = pipe_fd[0];
					cmd->is_heredoc = 1;
				}
				tmp = tmp->next;
			}
			free_heredoc_list(heredocs);
		}
		cmd = cmd->next;
	}
	signal(SIGINT,sigint_handler);
}

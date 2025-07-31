/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:20:14 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/31 18:18:12 by teraslan         ###   ########.fr       */
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

int	is_heredoc(char *input)
{
	int	i;
	int	inside_squote;
	int	inside_dquote;

	i = 0;
	inside_squote = 0;
	inside_dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !inside_dquote)
			inside_squote = !inside_squote;
		else if (input[i] == '\"' && !inside_squote)
			inside_dquote = !inside_dquote;
		else if (!inside_squote && !inside_dquote)
		{
			if (input[i] == '<' && input[i + 1] == '<')
				return (1);
		}
		i++;
	}
	return (0);
}

t_heredoc	*get_all_heredocs(char *input)
{
	t_heredoc	*head;
	t_heredoc	*last;
	int			i;
	int			in_squote;
	int			in_dquote;

	head = NULL;
	last = NULL;
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (input[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		if (!in_squote && !in_dquote
			&& input[i] == '<' && input[i + 1] == '<')
			add_heredoc_node(&head, &last, input, &i);
		else
			i++;
	}
	return (head);
}

void	check_heredoc_and_setup(t_command *command)
{
	t_command	*cmd;
	t_heredoc	*heredocs;

	cmd = command;
	while (cmd)
	{
		if (is_heredoc(cmd->tmp->input) && cmd->heredoc_fd == -1)
		{
			heredocs = get_all_heredocs(cmd->tmp->input);
			process_heredoc_list(cmd, heredocs);
			free_heredoc_list(heredocs);
			if (g_signal_status == 130)
				return ;
		}
		cmd = cmd->next;
	}
	signal(SIGINT, sigint_handler);
}

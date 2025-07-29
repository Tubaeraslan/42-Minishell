/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_function.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 13:31:35 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/29 13:09:41 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_error(t_command *command, const char *msg)
{
	printf("%s\n", msg);
	command->parsing_error = 1;
	command->last_exit_code = 2;
}

int	infile_error(t_command *cmd, char *file, const char *msg, int ex_c)
{
	if (!cmd->error_printed)
	{
		ft_putstr_fd("minishell: ", 2);
		if (file)
			ft_putstr_fd(file, 2);
		ft_putstr_fd((char *)msg, 2);
		cmd->error_printed = 1;
	}
	cmd->parsing_error = 1;
	cmd->last_exit_code = ex_c;
	return (-1);
}

int	outfile_error(t_command *cmd, char *f, const char *msg, int ex_c)
{
	if (!cmd->error_printed)
	{
		ft_putstr_fd("minishell: ", 2);
		if (f)
		{
			ft_putstr_fd(f, 2);
			ft_putstr_fd(": ", 2);
		}
		ft_putstr_fd((char *)msg, 2);
		cmd->error_printed = 1;
	}
	cmd->parsing_error = 1;
	cmd->last_exit_code = ex_c;
	return (-1);
}

void	copy_tokens(t_command *n_cmd, char **tokens, int start, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		n_cmd->tokens[j] = ft_strdup(tokens[start + j]);
		j++;
	}
}

int	has_any_heredoc(t_command *cmd)
{
	t_command	*cur;

	cur = cmd;
	while (cur)
	{
		if (is_heredoc(cur->tmp->input))
			return (1);
		cur = cur->next;
	}
	return (0);
}

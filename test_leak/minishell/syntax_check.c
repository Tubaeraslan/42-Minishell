/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 19:06:19 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/25 16:58:30 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_syntax(char *input)
{
	int	i;
	int	single_flag;
	int	double_flag;

	i = 0;
	single_flag = 0;
	double_flag = 0;
	while (input[i])
	{
		if (input[i] == '\'' && double_flag == 0)
			single_flag = !single_flag;
		else if (input[i] == '\"' && single_flag == 0)
			double_flag = !double_flag;
		i++;
	}
	if (single_flag || double_flag)
		return (0);
	return (1);
}

static int	is_invalid_redirect(char *input, int i)
{
	int	j;

	if ((input[i] == '<' || input[i] == '>')
		&&input[i] == input[i + 1] && input[i + 1] == input[i + 2])
		return (1);
	if ((input[i] == '<' || input[i] == '>') && input[i + 1] == '\0')
		return (1);
	if ((input[i] == '<' || input[i] == '>')
		&& (input[i + 1] == ' ' || input[i + 1] == '\0'))
	{
		j = i + 1;
		while (input[j] == ' ')
			j++;
		if (input[j] == '\0' || input[j] == '<' || input[j] == '>')
			return (1);
	}
	return (0);
}

int	check_redirects(char *input)
{
	int	i;
	int	single_flag;
	int	double_flag;

	i = 0;
	single_flag = 0;
	double_flag = 0;
	while (input[i])
	{
		if (input[i] == '\'' && double_flag == 0)
			single_flag = !single_flag;
		else if (input[i] == '\"' && single_flag == 0)
			double_flag = !double_flag;
		if (!single_flag && !double_flag)
		{
			if (is_invalid_redirect(input, i))
				return (0);
		}
		i++;
	}
	return (1);
}

int	check_syntax_errors(t_command *command)
{
	if (is_valid_syntax(command->tmp->input) == 0)
		return (parse_error(command, "syntax error: unclosed quote"), 0);
	if (check_pipe(command->tmp->input) == 0)
		return (parse_error(command,
				"syntax error near unexpected token `|'"), 0);
	if (check_redirects(command->tmp->input) == 0)
		return (parse_error(command,
				"syntax error near unexpected token `newline'"), 0);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:38:59 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/19 13:16:24 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_in_quotes(char c, int *single_flag, int *double_flag)
{
	if (c == '\'' && *double_flag == 0)
		*single_flag = !(*single_flag);
	else if (c == '\"' && *single_flag == 0)
		*double_flag = !(*double_flag);
	return (*single_flag || *double_flag);
}

static int	has_consecutive_pipes(char *input, int i)
{
	int	j;

	j = i - 1;
	while (j >= 0 && input[j] == ' ')
		j--;
	if (j >= 0 && input[j] == '|')
		return (1);
	return (0);
}

static int	is_invalid_pipe(char *input)
{
	int	i;
	int	single_flag;
	int	double_flag;

	i = 0;
	single_flag = 0;
	double_flag = 0;
	while (input[i] == ' ')
		i++;
	if (input[i] == '|')
		return (1);
	while (input[i])
	{
		if (!is_in_quotes(input[i], &single_flag, &double_flag))
		{
			if (input[i] == '|' && has_consecutive_pipes(input, i))
				return (1);
		}
		i++;
	}
	return (0);
}

static int	ends_with_pipe(char *input)
{
	int	i;

	i = ft_strlen(input) -1;
	while (i >= 0 && input[i] == ' ')
		i--;
	if (i >= 0 && input[i] == '|')
		return (1);
	return (0);
}

int	check_pipe(char *input)
{
	if (is_invalid_pipe(input))
		return (0);
	if (ends_with_pipe(input))
		return (0);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 13:26:20 by teraslan          #+#    #+#             */
/*   Updated: 2025/05/31 16:22:58 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_valid_syntax(char *input)
{
	int	i;
	int single_flag;
	int double_flag;

	i = 0;
	single_flag = 0;
	double_flag = 0;
	while (input[i])
	{
		if (input[i] == '\'' && double_flag == 0)
			single_flag = !single_flag;
		else if(input[i] == '\"' && single_flag == 0)
			double_flag = !double_flag;
		i++;
	}
	if (single_flag || double_flag)
		return 0;
	return 1;
}

static int ends_with_pipe(char *input)
{
	int i;

	i = ft_strlen(input) -1;
	while (i >= 0 && input[i] == ' ')
		i--;
	if (i >= 0 && input[i] == '|')
		return 1;
	return 0;
}

int check_pipe(char *input)
{
	int i;
	int j;
	int single_flag;
	int double_flag;

	i = 0;
	j = 0;
	single_flag = 0;
	double_flag = 0;
	
	while (input[i] == ' ')
		i++;
	if (input[i] == '|')
		return 0;
	while (input[i])
	{
		if (input[i] == '\'' && double_flag == 0)
			single_flag = !single_flag;
		else if(input[i] == '\"' && single_flag == 0)
			double_flag = !double_flag;
		if (input[i] == '|' && !single_flag && !double_flag)
		{
			j = i -1;
			while (j >= 0 && input[j] == ' ')
				j--;
			if (j >= 0 && input[j] == '|')
				return 0;
		}
		i++;
	}
	if (ends_with_pipe(input) == 1)
		return 0;
	return 1;
}

int check_redirects(char *input)
{
	int i;
	int single_flag;
	int double_flag;

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
			// 3 veya daha fazla
			if ((input[i] == '<' || input[i] == '>') &&
				input[i] == input[i + 1] && input[i + 1] == input[i + 2])
				return 0;
			// tek < veya > sonunda kaldıysa
			if ((input[i] == '<' || input[i] == '>') && input[i + 1] == '\0')
				return 0;
			// yönlendirme sonrası boşluk varsa ve sonra yine yönlendirme varsa
			if ((input[i] == '<' || input[i] == '>') &&
				(input[i + 1] == ' ' || input[i + 1] == '\0'))
				{
					int j = i + 1;
					while (input[j] == ' ')
						j++;
					if (input[j] == '\0' || input[j] == '<' || input[j] == '>')
						return 0;
				}
		}
		i++;
	}
	return 1;
}
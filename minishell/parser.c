/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:11:11 by teraslan          #+#    #+#             */
/*   Updated: 2025/05/05 17:56:01 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_valid_syntax(char *input)
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
		else if(input[i] == '\"' && single_flag == 0)
			double_flag = !double_flag;

		i++;
	}

	if (single_flag || double_flag) 
	{
		return (0);

	}
	return 1;
}

int ends_with_pipe(char * input)
{
	int i;

	i = ft_strlen(input) -1;
	while (i >= 0 && input[i] == ' ')
		i--;
	if (i >= 0 && input[i] == '|')
		return 0;
	return 0;
}

int check_pipe(char *input)
{
	int i;
	int j;
	int single_flag;
	int double_flag;

	j = 0;
	i = 0;
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
	if(ends_with_pipe(input) == 0)
		return 0;
	return 1;
}

int check_redirect(char *input)
{
	int i;
	int count;
	char c;
	
	i = 0;
	count = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>')
		{
			count = 1;
			c = input[i];
			if (input[i+1] == c)
			{
				count++;
				i++;
			}
			if (input[i + 1] == c)
				return 0;
			i++;
			while (input[i] == ' ')
				i++;
			if (input[i] == '\0' || input[i] == '|' || input[i] == '<' || input[i] == '>')
				return 0;	
		}
		else
			i++;
	}
	return 1;
}

void parse_input(t_data *shell)
{
	//t_command *command;
	//t_command *cmd_lst;
	//char **tokens;

	//boşsa işlem yapma
	if (!shell->input || shell->input[0] == '\0')
		return;
	//1) syntax kontrolü
	/*
	açık kalan tırnak - yanlış pipe - eksik komut
	*/
	if (is_valid_syntax(shell->input) == 0)
	{
		printf("syntax error: unclosed quote\n");
		return;
	}
	if (check_pipe(shell->input) == 0)
	{
		printf("syntax error near unexpected token `|'\n");
		return;
	}
	if(check_redirect(shell->input) == 0)
	{
		printf("syntax error near unexpected token `newline'\n");
		return;
	}
	
	
	//2)tokenleri ayır (space, | , <, >, <<, >>)
	/*
		girdiyi karakter karakter işle
		char **tokens = lexer(shell->input);
		if(!tokens){
			return;
		}
	*/
	//3)parse işlemi
	/*
		cmd_list = command_lst(tokens);
	*/
	//4)execute işlemi için kaydet
	/*
		shell->cmd = cmd_lst;
	*/

	//free_tokens(tokens);
}
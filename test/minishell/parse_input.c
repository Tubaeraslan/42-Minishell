/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:37:12 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/07 16:01:50 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_token(t_command *command, char *buffer)
{
	char **tmp;
	int i = 0;

	// Yeni token dizisi için yer ayır (eski + 1 yeni + NULL için 1)
	tmp = malloc(sizeof(char *) * (command->token_count + 2));
	if (!tmp)
	{
		perror("malloc failed");
		exit(1);
	}

	// Eski token'ları yeni dizinin içine kopyala
	while (i < command->token_count)
	{
		tmp[i] = command->tokens[i];
		i++;
	}

	// Yeni token'ı ekle
	tmp[i++] = buffer;
	tmp[i] = NULL;

	// Önceki token dizisi varsa ama içindekiler zaten tmp'ye aktarıldı
	// sadece token dizisinin kendisini free() etmek yeterli olur
	if (command->tokens)
		free(command->tokens);

	command->tokens = tmp;
	command->token_count++;
}

static void process_remaining_buffer(t_command *command, t_tokenizer *tk)
{
	if (tk->j > 0)
	{
		tk->buffer[tk->j] = '\0';
		add_token(command, ft_strdup(tk->buffer));
		tk->j = 0;
	}
}

static void process_char(t_tokenizer *tk, t_command *command)
{
	const char *src = tk->src;
	int *i = &tk->i;

	if (src[*i] == '\'' || src[*i] == '"')
	{
		if (process_quotes(tk))
			return ;
	}
	else if (!tk->inside_quotes && src[*i] == ' ')
	{
		process_space(command, tk);
		(*i)++;
		return ;
	}
	else if (!tk->inside_quotes && (src[*i] == '>' || src[*i] == '<' || src[*i] == '|'))
	{
		int len = process_operator(command, tk);
		(*i) += len;
		return ;
	}
	tk->buffer[(tk->j)++] = src[(*i)++];
}

static void token(t_command *command)
{
	t_tokenizer *tk = &command->tokenizer;

	clear_tokens(command);
	tk->i = 0;
	tk->j = 0;
	tk->inside_quotes = 0;
	tk->char_quote = 0;
	tk->src = command->tmp->input;

	while (tk->src[tk->i])
		process_char(tk, command);

	process_remaining_buffer(command, tk);
}

void parse_input(t_command *command)
{
	if (!command->tmp->input || command->tmp->input[0] == '\0')
		return;
	if (is_valid_syntax(command->tmp->input) == 0)
	{
		printf("syntax error: unclosed quote\n");
		return;
	}
	if (check_pipe(command->tmp->input) == 0)
	{
		printf("syntax error near unexpected token `|'\n");
		return;
	}
	if (check_redirects(command->tmp->input) == 0)
	{
		printf("syntax error near unexpected token `newline'\n");
		return;
	}
	expand_variables(command);
	token(command);
	parsing(command);
}


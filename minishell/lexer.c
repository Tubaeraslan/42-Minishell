/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:00:48 by teraslan          #+#    #+#             */
/*   Updated: 2025/05/31 16:29:17 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void clear_tokens(t_command *command)
{
	int i = 0;

	if (!command->tokens)
		return;

	while (i < command->token_count)
	{
		free(command->tokens[i]);
		i++;
	}
	free(command->tokens);
	command->tokens = NULL;
	command->token_count = 0;
}

static void add_token(t_command *command, char *buffer)
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

static int handle_operators(t_command *command, char *src)
{
	if (src[0] == '>' && src[1] == '>')
	{
		add_token(command,ft_strdup(">>"));
		return 2;
	}
	else if (src[0] == '<' && src[1] == '<')
	{
		add_token(command, ft_strdup("<<"));
		return 2;
	}
	else if (src[0] == '>')
	{
		add_token(command, ft_strdup(">"));
		return 1;
	}
	else if (src[0] == '<')
	{
		add_token(command, ft_strdup("<"));
		return 1;
	}
	else if (src[0] == '|')
	{
		add_token(command, ft_strdup("|"));
		return 1;
	}
	return 0;
}

void token(t_command *command)
{
	int i;
	int j;
	char *src;
	char char_quote;
	char buffer[1024];
	int inside_quotes;

	clear_tokens(command);
	i = 0;
	j = 0;
	inside_quotes = 0;
	char_quote = 0;
	src = command->tmp->input;
	while (src[i])
	{
		if (src[i] == '\'' || src[i] == '"')
		{
			if (!inside_quotes)
			{
				inside_quotes = 1;
				char_quote = src[i];
			}
			else if(char_quote == src[i])
			{
				inside_quotes = 0;
				char_quote = 0;
			}
		}
		else if(!inside_quotes && (src[i] == ' ' || src[i] == '|'
			|| src[i] == '<' || src[i] == '>')){
			{
				if (j > 0)
				{
					buffer[j] = '\0';
					add_token(command, ft_strdup(buffer));
					j = 0;
				}
				i += handle_operators(command,&src[i]);
			}
		}
		else
		{
			buffer[j] = src[i];
			j++;
		}
		i++;
	}
	if (j > 0)
	{
		buffer[j] = '\0';
		add_token(command, ft_strdup(buffer));
	}
}
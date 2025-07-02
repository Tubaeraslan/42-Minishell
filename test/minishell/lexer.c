/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:11:17 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/02 14:36:38 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int handle_operators(t_command *command, char *src)
{
	if (src[0] == '>' && src[1] == '>')
	{
		add_token(command, ft_strdup(">>"));
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

void clear_tokens(t_command *command)
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

int process_quotes(t_tokenizer *tk)
{
	const char *src = tk->src;
	int *i = &tk->i;

	if (!tk->inside_quotes)
	{
		tk->inside_quotes = 1;
		tk->char_quote = src[*i];
		(*i)++;
		return 1;
	}
	else if (src[*i] == tk->char_quote)
	{
		tk->inside_quotes = 0;
		tk->char_quote = 0;
		(*i)++;
		return 1;
	}
	return 0;
}

void process_space(t_command *command, t_tokenizer *tk)
{
	if (tk->j > 0)
	{
		tk->buffer[tk->j] = '\0';
		add_token(command, ft_strdup(tk->buffer));
		tk->j = 0;
	}
}

int process_operator(t_command *command, t_tokenizer *tk)
{
	if (tk->j > 0)
	{
		tk->buffer[tk->j] = '\0';
		add_token(command, ft_strdup(tk->buffer));
		tk->j = 0;
	}
	return handle_operators(command, (char *)&tk->src[tk->i]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:37:12 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/19 19:42:20 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_command *command, char *buffer)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = malloc(sizeof(char *) * (command->token_count + 2));
	if (!tmp)
	{
		perror("malloc failed");
		exit(1);
	}
	while (i < command->token_count)
	{
		tmp[i] = ft_strdup(command->tokens[i]);
		i++;
	}
	tmp[i++] = buffer;
	tmp[i] = NULL;
	if (command->tokens)
	{
		for (int j = 0; j < command->token_count; j++)
		{
			free(command->tokens[j]);  // içteki strdup edilmiş string'leri sil
			command->tokens[j] = NULL;
		}
		free(command->tokens);        // sonra diziyi sil
		command->tokens = NULL;
	}
	command->tokens = tmp;
	command->token_count++;
}

static void	process_remaining_buffer(t_command *command, t_tokenizer *tk)
{
	if (tk->j > 0)
	{
		tk->buffer[tk->j] = '\0';
		add_token(command, ft_strdup(tk->buffer));
		tk->j = 0;
	}
}

static void	process_char(t_tokenizer *tk, t_command *command)
{
	const char	*src;
	int			*i;
	int			len;

	src = tk->src;
	i = &tk->i;
	if (src[*i] == '\'' || src[*i] == '"')
	{
		if (process_quotes(tk))
			return ;
	}
	else if (!tk->inside_quotes && (src[*i] == ' ' || src[*i] == '\t'))
	{
		process_space(command, tk);
		(*i)++;
		return ;
	}
	else if (!tk->inside_quotes && (src[*i] == '>'
			|| src[*i] == '<' || src[*i] == '|'))
	{
		len = process_operator(command, tk);
		(*i) += len;
		return ;
	}
	tk->buffer[(tk->j)++] = src[(*i)++];
}

static void	token(t_command *command)
{
	t_tokenizer	*tk;

	tk = &command->tokenizer;
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

void	parse_input(t_command *command)
{
	if (!command->tmp->input || command->tmp->input[0] == '\0')
		return ;
	if (is_valid_syntax(command->tmp->input) == 0)
	{
		parse_error(command, "syntax error: unclosed quote");
		return ;
	}
	if (check_pipe(command->tmp->input) == 0)
	{
		parse_error(command, "syntax error near unexpected token `|'");
		return ;
	}
	if (check_redirects(command->tmp->input) == 0)
	{
		parse_error(command, "syntax error near unexpected token `newline'");
		return ;
	}
	expand_variables(command);
	token(command);
	parsing(command);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:37:12 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/23 17:55:38 by teraslan         ###   ########.fr       */
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

char	*get_heredoc_limiter(char *input)
{
	char	**tokens = ft_split(input, ' ');
	int		i = 0;
	char	*limiter = NULL;

	while (tokens[i])
	{
		if (strcmp(tokens[i], "<<") == 0 && tokens[i + 1])
		{
			limiter = ft_strdup(tokens[i + 1]);
			break ;
		}
		i++;
	}
	//ft_free_split(tokens); // belleği temizle
	return (limiter);
}

int	has_heredoc(char *input)
{
	return (ft_strnstr(input, "<<", ft_strlen(input)) != NULL);
}

void	parse_input(t_command *command)
{
	if (!command->tmp->input || command->tmp->input[0] == '\0')
		return ;

	// Heredoc varsa önce oku
	if (has_heredoc(command->tmp->input))
	{
		command->heredoc_limiter = get_heredoc_limiter(command->tmp->input); // "< <EOF" gibi kısımdan limiter al
		if (command->heredoc_limiter)
			setup_heredoc(command);
	}

	// Sonra syntax hatalarına bak
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:37:12 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/24 18:25:35 by teraslan         ###   ########.fr       */
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

char *get_heredoc_limiter(char *input)
{
    int i = 0;
    int inside_single_quote = 0;
    int inside_double_quote = 0;

    while (input[i])
    {
        if (input[i] == '\'' && !inside_double_quote)
            inside_single_quote = !inside_single_quote;
        else if (input[i] == '\"' && !inside_single_quote)
            inside_double_quote = !inside_double_quote;

        if (!inside_single_quote && !inside_double_quote)
        {
            if (input[i] == '<' && input[i + 1] == '<')
            {
                i += 2;
                // Boşlukları atla
                while (input[i] == ' ' || input[i] == '\t')
                    i++;

                int start = i;
                while (input[i] && input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
                    i++;

                int len = i - start;
                if (len == 0)
                    return NULL;

                char *limiter = malloc(len + 1);
                if (!limiter)
                    return NULL;
                strncpy(limiter, input + start, len);
                limiter[len] = '\0';

                return limiter;
            }
        }
        i++;
    }
    return NULL;
}

int	has_heredoc(char *input)
{
	return (ft_strnstr(input, "<<", ft_strlen(input)) != NULL);
}

void	parse_input(t_command *command)
{
	if (!command->tmp->input || command->tmp->input[0] == '\0')
		return ;
	t_command *cmd = command;
	// Heredoc varsa önce oku
	while (cmd)
	{
		if (has_heredoc(cmd->tmp->input) && cmd->heredoc_fd == -1)
		{
			cmd->heredoc_limiter = get_heredoc_limiter(cmd->tmp->input);
			if (cmd->heredoc_limiter)
				setup_heredoc(cmd);
		}
		cmd = cmd->next;
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

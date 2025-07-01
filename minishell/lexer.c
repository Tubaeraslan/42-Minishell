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

static int process_quotes(t_tokenizer *tk)
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

static void process_space(t_command *command, t_tokenizer *tk)
{
	if (tk->j > 0)
	{
		tk->buffer[tk->j] = '\0';
		add_token(command, ft_strdup(tk->buffer));
		tk->j = 0;
	}
}

static int process_operator(t_command *command, t_tokenizer *tk)
{
	if (tk->j > 0)
	{
		tk->buffer[tk->j] = '\0';
		add_token(command, ft_strdup(tk->buffer));
		tk->j = 0;
	}
	return handle_operators(command, (char *)&tk->src[tk->i]);
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

void token(t_command *command)
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

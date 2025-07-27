/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:37:12 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/27 12:46:08 by teraslan         ###   ########.fr       */
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
		all_free(command);
		exit(1);
	}
	while (i < command->token_count)
	{
		tmp[i] = ft_strdup(command->tokens[i]);
		i++;
	}
	tmp[i++] = buffer;
	tmp[i] = NULL;
	free_old_tokens(command);
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

int has_any_heredoc(t_command *cmd)
{
	t_command *cur = cmd;
	while (cur)
	{
		if (has_heredoc(cur->tmp->input)) // zaten varsa böyle bir fonksiyonun
			return 1;
		cur = cur->next;
	}
	return 0;
}

void	parse_input(t_command *command)
{
	if (!command->tmp->input || command->tmp->input[0] == '\0')
		return ;
	if (ft_strncmp(command->tmp->input, "\"\"", 2) == 0)
	{
		ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
		command->last_exit_code = 127;
		return ;
	}
	check_heredoc_and_setup(command);
	if (g_signal_status == 130)
	{
		command->last_exit_code = 130;
		g_signal_status = 0;
		free(command->tmp->input);
		command->tmp->input = NULL;
		return ;
	}
	if (has_any_heredoc(command) && command->heredoc_fd == -1)
	{
		command->last_exit_code = 130;
		free(command->tmp->input);
		command->tmp->input = NULL;
		return ;
	}
	if (!check_syntax_errors(command))
		return ;
	expand_variables(command);
	token(command);
	parsing(command);
}

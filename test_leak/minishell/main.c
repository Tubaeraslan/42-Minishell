/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:04:40 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/19 16:55:23 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_command(t_command *command, t_data *shell, char **envp)
{
	command->tmp = shell;
	command->tmp->input = NULL;
	command->tmp->env = ft_env_dup(envp);
	command->token_count = 0;
	command->tokens = NULL;
	command->args = NULL;
	command->is_pipe = 0;
	command->next = NULL;
	command->cmd = NULL;
	command->redirects = NULL;
	command->infile = NULL;
	command->outfile = NULL;
	command->heredoc_limiter = NULL;
	command->is_append = 0;
	command->is_heredoc = 0;
	command->heredoc_fd = 0;
	command->last_exit_code = 0;
	command->parsing_error = 0;
	command->error_printed = 0;
	ft_bzero(&command->tokenizer, sizeof(t_tokenizer));
	ft_bzero(&command->collector, sizeof(s_GarbageCollector));
}

static void	init_pointer(t_command **command, t_data **shell)
{
	*shell = malloc(sizeof(t_data));
	if (!*shell)
		exit(1);
	*command = malloc(sizeof(t_command));
	if (!*command)
		exit(1);
}

static int	read_and_prepare_input(t_command *command)
{
	char	*input;

	input = readline("minishell$ ");
	if (!input)
	{
		printf("exit\n");
		return (1);
	}
	add_history(input);
	if (command->tmp->input)
	{
		free(command->tmp->input);
		command->tmp->input = NULL;
	}
	command->tmp->input = ft_strdup(input);
	free(input);
	if (!command->tmp->input)
	{
		perror("ft_strdup");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*shell;
	t_command	*command;

	(void)argc;
	(void)argv;
	init_pointer(&command, &shell);
	init_command(command, shell, envp);
	handle_signals();
	while (1)
	{
		if (read_and_prepare_input(command))
			break ;
		parse_input(command);
		execute_commands(command);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <ican@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 13:06:40 by teraslan          #+#    #+#             */
/*   Updated: 2025/06/02 11:09:09 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_command(t_command *command,t_data *shell,char **envp)
{
	command->tmp = shell;
	command->tmp->input = NULL;
	command->tmp->env = envp;
	command->token_count = 0;
	command->tokens = NULL;
	command->args = NULL;
	command->is_pipe = 0;
	command->next = NULL;
	command->cmd = NULL;
	command->redirects = NULL;
	command->infile = NULL;
	command->outfile = NULL;
	command->is_append = 0;
}

void sigint_handler(int sig)
{
	(void)sig;
	write(2,"\n",1);
	rl_replace_line("",0);  //mevcut satırı temizle
	rl_on_new_line();  //readline a yeni satır olduğunu bildir
	rl_redisplay();   //promptu ve satırı yeniden yaz
}

void handle_signals()
{
	//ctrl-c
	signal(SIGINT, sigint_handler);
	//ctrl-\'
	signal(SIGQUIT,SIG_IGN);
}

int main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	shell;
	t_command	command;

	(void)argc;
	(void)argv;
	init_command(&command,&shell,envp);
	handle_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		else
			add_history(input);
		command.tmp->input = input;
		//input kontrol
		printf("Kullanici: %s\n",command.tmp->input);

		//parse
		parse_input(&command);
		execute_commands(&command);
	}
	
	return 0;
}

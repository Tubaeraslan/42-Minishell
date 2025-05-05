/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:58:44 by teraslan          #+#    #+#             */
/*   Updated: 2025/05/05 14:29:45 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	sigint_handler(int sig)
{
	(void)sig;
	write(2,"\n",1);
	rl_replace_line("",0);  //mevcut satırı temizle
	rl_on_new_line();  //readline a yeni satır olduğunu bildir
	rl_redisplay();   //promptu ve satırı yeniden yaz
}


void	handle_signals()
{
	//ctrl-c
	signal(SIGINT ,sigint_handler);

	// ctrl-\ '
	signal(SIGQUIT, SIG_IGN);
}

int main(int argc, char **argv ,char **envp)
{
	char *input;
	t_data shell;
	t_command command;

	shell.env = envp;
	(void)argc;
	(void)argv;
	
	//ctrl-c  ctrk-\'
	handle_signals();
	
	while (1)
	{
		input = readline("minishell$ ");
		command.tmp = &shell;
		if (!input)
		{
			printf("exit\n");
			break;
		}
		else
		{
			//komutları tutmak için
			add_history(input);
		}
		shell.input = input;
		printf("Kullanici: %s\n",shell.input);

		//parser
		parse_input(&shell);

		//execute
		
		//execute_commands(&command);

		free(input);
		shell.input = NULL;
	}

	//struct free leri için
	//clean_shell(&shell);
	
	return 0;
}

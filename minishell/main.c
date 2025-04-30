/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:58:44 by teraslan          #+#    #+#             */
/*   Updated: 2025/04/30 17:15:33 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void signint_handler(int sig){
	(void)sig;
	printf("\n");
	rl_replace_line("", 0);  //giriş satırını (kullanıcının yazdığı şeyi) temizler.
	rl_on_new_line(); // Prompt'u yeniden konumlandırmak için gerekli.
	rl_redisplay(); // Prompt'u (örneğin "minishell$ ") tekrar ekrana yazar.
}


void	handle_signals()
{
	//ctrl-c
	signal(SIGINT ,signint_handler);

	// ctrl-\ '
	signal(SIGQUIT, SIG_IGN);
}

int main(int argc, char **argv ,char **envp)
{
	char *input;
	t_data shell;
	t_command command;

	rl_catch_signals = 0; 
	shell.env = envp;
	(void)argc;
	(void)argv;
	
	//ctrl-c  ctrl-d   ctrk-\'
	handle_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
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
		//execute_commands(&command)

		free(input);
		shell.input = NULL;
	}

	//struct free leri için
	//clean_shell(&shell);
	
	return 0;
}

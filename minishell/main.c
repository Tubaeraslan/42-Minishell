/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:58:44 by teraslan          #+#    #+#             */
/*   Updated: 2025/04/30 15:28:15 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv ,char **envp)
{
	char *input;
	t_data shell;

	shell.env = envp;
	(void)argc;
	(void)argv;
	
	//ctrl-c  ctrl-d   ctrk-\'
	//handle_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		else
		{
			//komutları tutmak için
			//add_history(input);
		}
		shell.input = input;
		printf("Kullanici: %s\n",shell.input);

		//parser
		//parse_input(&shell);

		//execute
		//execute_commands(&shell)

		free(input);
		shell.input = NULL;
	}

	//struct free leri için
	//clean_shell(&shell);
	
	return 0;
}

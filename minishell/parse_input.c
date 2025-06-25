/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 13:23:51 by teraslan          #+#    #+#             */
/*   Updated: 2025/06/22 17:43:15 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_input(t_command *command)
{
	//boşsa işlem yapma
	if (!command->tmp->input || command->tmp->input[0] == '\0')
		return;
	//syntax kontrolü açık tınak-yanlış pipe-eksikkomut
	if (is_valid_syntax(command->tmp->input) == 0)
	{
		printf("syntax error: unclosed quote\n");
		return;
	}
	if (check_pipe(command->tmp->input) == 0)
	{
		printf("syntax error near unexpected token `|'\n");
		return;
	}
	if (check_redirects(command->tmp->input) == 0)
	{
		printf("syntax error near unexpected token `newline'\n");
		return;
	}

	//tokenları ayır(space, "" , |,redirect)
	token(command);
	//token kontrol
	int k = 0;
	while (command->tokens && command->tokens[k])
	{
    	printf("Token[%d]: %s\n", k, command->tokens[k]);
    	k++;
	}

	//parse işlemi
	//tokenları grupla- pipe ı bul - redirectionları
	//sıralama 1)pipe gördüğünde yeni t_command başlat
	//2)redirection varsa önceki komutla bağla
	//3)geriye kalanlar argüman execve()kullanmak için lazım
	parsing(command);

	//parse kontrol
	// İlk komutu yazdır
	int i;
	t_command *tmp;
	tmp = command;

    while (command)
    {
        printf("-------- AFTER PARSING --------\n");
        if (command->cmd)
            printf("Command: %s\n", command->cmd);
        else
            printf("Command: (null)\n");

        printf("Arguments:\n");
        if (command->args)
        {
            i = 0;
            while (command->args[i])
            {
                printf("  Arg[%d]: %s\n", i, command->args[i]);
                i++;
            }
        }
        else
            printf("  (no arguments)\n");

        if (command->infile)
            printf("Infile: %s\n", command->infile);
        else
            printf("Infile: (null)\n");

        if (command->outfile)
        {
            printf("Outfile: %s\n", command->outfile);
            if (command->is_append)
                printf("Append mode: Yes (>>)\n");
            else
                printf("Append mode: No (>)\n");
        }
        else
            printf("Outfile: (null)\n");

        printf("Pipe: %s\n", command->is_pipe ? "Yes (|)" : "No");
        printf("------------------------\n");

        // Sonraki komuta geç
        command = command->next;
	}
	while (tmp)
	{
    	printf("tmp->cmd = %s\n", tmp->cmd ? tmp->cmd : "(null)");
    	tmp = tmp->next;
	}

}


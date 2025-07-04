/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 13:23:51 by teraslan          #+#    #+#             */
/*   Updated: 2025/06/30 19:56:27 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_token(t_command *command, char *buffer)
{
	char **tmp;
	int i = 0;

	// Yeni token dizisi için yer ayır (eski + 1 yeni + NULL için 1)
	tmp = malloc(sizeof(char *) * (command->token_count + 2));
	if (!tmp)
	{
		perror("malloc failed");
		exit(1);
	}

	// Eski token'ları yeni dizinin içine kopyala
	while (i < command->token_count)
	{
		tmp[i] = command->tokens[i];
		i++;
	}

	// Yeni token'ı ekle
	tmp[i++] = buffer;
	tmp[i] = NULL;

	// Önceki token dizisi varsa ama içindekiler zaten tmp'ye aktarıldı
	// sadece token dizisinin kendisini free() etmek yeterli olur
	if (command->tokens)
		free(command->tokens);

	command->tokens = tmp;
	command->token_count++;
}

char *get_env_value(char **env, char *key)
{
	int i = 0;
	size_t key_len = ft_strlen(key);

	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return env[i] + key_len + 1; // '=' den sonrası value
		i++;
	}
	return NULL;
}


void expand_variable_from_env(t_expand *ex, t_command *command)
{
	ex->i++;
	char varname[256];
	int k = 0;

	while (ex->input[ex->i] && (ft_isalnum(ex->input[ex->i]) || ex->input[ex->i] == '_'))
		varname[k++] = ex->input[ex->i++];
	varname[k] = '\0';

	char *value = get_env_value(command->tmp->env, varname);
	if (value)
	{
		int m = 0;
		while (value[m])
			ex->new_input[ex->j++] = value[m++];
	}
}

void expand_loop(t_expand *ex, t_command *command)
{
	while (ex->input[ex->i])
	{
		if (ex->input[ex->i] == '\'' && !ex->double_quote)
		{
			ex->single_quote = !ex->single_quote;
			ex->new_input[ex->j++] = ex->input[ex->i++];
		}
		else if (ex->input[ex->i] == '"' && !ex->single_quote)
		{
			ex->double_quote = !ex->double_quote;
			ex->new_input[ex->j++] = ex->input[ex->i++];
		}
		else if (ex->input[ex->i] == '$' && !ex->single_quote)
			expand_variable_from_env(ex, command);
		else
			ex->new_input[ex->j++] = ex->input[ex->i++];
	}
}

void expand_variables(t_command *command)
{
	t_expand *ex;

	ex = malloc(sizeof(t_expand));
	if (!ex)
		return;
	ex->input = command->tmp->input;
	ex->new_input = malloc(4096);
	if (!ex->new_input)
		return;
	ex->i = 0;
	ex->j = 0;
	ex->single_quote = 0;
	ex->double_quote = 0;

	expand_loop(ex, command);

	ex->new_input[ex->j] = '\0';
	free(command->tmp->input);
	command->tmp->input = ft_strdup(ex->new_input);
	free(ex->new_input);
	free(ex);
}


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

	//$kontrolü
	expand_variables(command);

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


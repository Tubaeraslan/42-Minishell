/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 12:49:36 by teraslan          #+#    #+#             */
/*   Updated: 2025/05/11 15:06:39 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_token(t_command *command, char *buffer)
{
    char **tmp;
    int i;

    i = 0;

    // Bellek ayırırken command->tokens NULL ise, ilk kez bellek ayırıyoruz
    tmp = malloc(sizeof(char *) * (command->token_count + 2));
    if (!tmp)
    {
        // Bellek hatası durumunda uygun şekilde hata verip çıkabiliriz
        perror("malloc failed");
        exit(1);
    }

    // Eğer command->tokens daha önce belleği tahsis ettiyse, eski token'ları kopyalayalım
    while (i < command->token_count)
    {
        tmp[i] = command->tokens[i];
        i++;
    }

    // Yeni token'ı ekleyelim
    tmp[i++] = buffer;
    tmp[i] = NULL;

    // Eğer önceki token'lar var ise belleği serbest bırak
    if (command->tokens)
    {
        free(command->tokens);
    }

    // Yeni token dizisini kaydedelim
    command->tokens = tmp;
    command->token_count++;
}

int handle_operators(t_command *command, char *src)
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

void token(t_command *command){
	int i;
	int j;

	char *src = command->tmp->input;
	char char_with_quote;
	char buffer[1024];
	int inside_quotes;
	j = 0;
	i = 0;
	inside_quotes = 0;
	char_with_quote = 0;
	while (src[i])
	{
		if (src[i] == '\'' || src[i] == '"')
		{
			if (!inside_quotes)
			{
				inside_quotes = 1;
				char_with_quote = src[i];
			}
			else if(char_with_quote == src[i])
			{
				inside_quotes = 0;
				char_with_quote = 0;
			}
			
		}
		else if (!inside_quotes && (src[i] == ' ' || src[i] == '|' || src[i] == '<'
			|| src[i] == '>'))
		{
			if (j > 0)
			{
				buffer[j] = '\0';
				add_token(command, ft_strdup(buffer));
				j = 0;
			}
			i += handle_operators(command,&src[i]);
		}
		else
		{
			buffer[j] = src[i];
			j++;
		}
		i++;
	}
	if (j > 0)
	{
		buffer[j] = '\0';
		add_token(command, ft_strdup(buffer));
	}
	
}
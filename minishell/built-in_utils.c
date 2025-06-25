/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:43:03 by ican              #+#    #+#             */
/*   Updated: 2025/06/22 16:06:21 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int numeric_control(char *arg)
{
    int i;

    i = 0;
    if(!arg)
        return (0);

    while (arg[i] != '\0')
    {
        if(ft_isdigit(arg[i]) == 0)
            return (1);
        i++;
    }
    return(0);
}

void exit_program(t_command *cmd, int exit_code)
{
    (void)cmd;
    // Bellek temizliği
    //all_free(cmd);  // t_command içindeki argümanlar, path, heredoc vs. hepsi free edilmeli

    // readline geçmişi varsa temizle
    rl_clear_history();

    // Diğer sistem kaynakları (örneğin dosya descriptorları) varsa kapatılmalı

    exit(exit_code);
}

int is_valid(char *str)
{
	int i = 0;

	if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void update_env(char ***envp, char *arg)
{
	int i = 0;
	int key_len = 0;
	char *key;

	while (arg[key_len] && arg[key_len] != '=')
		key_len++;
	key = ft_substr(arg, 0, key_len);

	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, key_len) == 0 && (*envp)[i][key_len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(arg);
			free(key);
			return;
		}
		i++;
	}

	// yoksa yeni değişkeni ekle
	*envp = add_to_env(*envp, arg); // realloc + strdup ile eklenir
	free(key);
}
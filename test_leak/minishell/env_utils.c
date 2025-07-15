/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:27:03 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/11 18:42:55 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_env(char **env)
{
	int i = 0;

	if (!env)
		return;

	while (env[i])
		free(env[i++]);

	free(env);
}

char **add_to_env(char **env, char *new_var)
{
	int i = 0;
	char **new_env;

	// Mevcut ortamın eleman sayısını say
	while (env && env[i])
		i++;

	// Yeni ortam için 1 fazla yer ayır (yeni değişken + NULL)
	new_env = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);

	// Eski ortamı kopyala
	i = 0;
	while (env && env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}

	// Yeni değişkeni ekle
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;

	// Eski ortamı temizle
	free_env(env);

	return (new_env);
}


void update_env(char ***envp, char *arg)
{
	int i = 0;
	int key_len = 0;
	char *key;

	if (!arg) // NULL arguman gelirse erken çık
		return;

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
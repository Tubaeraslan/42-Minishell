/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_function.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:27:50 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/01 18:26:19 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_len(char **env)
{
	int i = 0;
	while (env && env[i])
		i++;
	return (i);
}

static void	swap(char **a, char **b)
{
	char *tmp = *a;
	*a = *b;
	*b = tmp;
}

char	**sort_env(char **env)
{
	int		len = env_len(env);
	char	**sorted;
	int		i, j;

	// Ortamın kopyasını oluştur
	sorted = (char **)malloc(sizeof(char *) * (len + 1));
	if (!sorted)
		return (NULL);

	i = 0;
	while (i < len)
	{
		sorted[i] = ft_strdup(env[i]);
		i++;
	}
	sorted[i] = NULL;

	// Basit bubble sort (küçük veri için yeterli)
	for (i = 0; i < len - 1; i++)
	{
		for (j = 0; j < len - i - 1; j++)
		{
			if (ft_strncmp(sorted[j], sorted[j + 1], ft_strlen(sorted[j]) + 1) > 0)
				swap(&sorted[j], &sorted[j + 1]);
		}
	}

	return (sorted);
}

char **ft_env_dup(char **envp)
{
	int i;
	char **env_copy;

	i = 0;
	while (envp && envp[i])
		i++;
	env_copy = malloc(sizeof(char *) * (i + 1));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (envp && envp[i])
	{
		env_copy[i] = ft_strdup(envp[i]);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
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

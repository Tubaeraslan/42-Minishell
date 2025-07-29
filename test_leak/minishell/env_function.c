/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_function.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:27:50 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/29 16:53:24 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_len(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

static void	bubble_sort(char **arr, int len)
{
	int		i;
	int		j;
	char	*tmp;
	size_t	n;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			n = ft_strlen(arr[j]);
			if (ft_strlen(arr[j + 1]) > n)
				n = ft_strlen(arr[j + 1]);
			if (ft_strncmp(arr[j], arr[j + 1], n + 1) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	**sort_env(char **env)
{
	int		len;
	char	**sorted;

	len = env_len(env);
	sorted = ft_env_dup(env);
	if (!sorted)
		return (NULL);
	bubble_sort(sorted, len);
	return (sorted);
}

char	**ft_env_dup(char **envp)
{
	int		i;
	char	**env_copy;

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
		if (!env_copy[i])
		{
			while (i > 0)
			{
				free(env_copy[i - 1]);
				i--;
			}
			free(env_copy);
			return (NULL);
		}
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

char	*get_env_value(char **env, char *key)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

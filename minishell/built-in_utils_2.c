/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 14:49:38 by teraslan          #+#    #+#             */
/*   Updated: 2025/06/22 15:55:05 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **ft_env_dup(char **envp)
{
    int i = 0;
    char **dup;

    while (envp && envp[i])
        i++;

    dup = malloc(sizeof(char *) * (i + 1));
    if (!dup)
        return (NULL);

    i = 0;
    while (envp && envp[i])
    {
        dup[i] = ft_strdup(envp[i]);
        i++;
    }
    dup[i] = NULL;
    return (dup);
}

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

void print_declare_line(char *env)
{
	int i = 0;

	while (env[i] && env[i] != '=')
		write(1, &env[i++], 1);
	if (env[i] == '=')
	{
		write(1, "=\"", 2);
		ft_putstr_fd(&env[i + 1], 1);
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

void print_env_sorted(char **envp)
{
	char **sorted = sort_env(envp); // alfabetik sırala
	int i = 0;

	while (sorted[i])
	{
		ft_putstr_fd("declare -x ", 1);
		print_declare_line(sorted[i]);
		i++;
	}
	//free_env(sorted); // malloc'la kopyalanmışsa freele
}

char	**remove_from_env(char **env, const char *var)
{
	int		i = 0, j = 0, count = 0;
	char	**new_env;
	int		len = ft_strlen(var);

	// 1. Geçerli eleman sayısını say
	while (env && env[count])
		count++;

	// 2. Yeni env dizisi için yer ayır (maksimum aynı uzunlukta olur)
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);

	// 3. Eski diziyi dolaş, eşleşmeyenleri kopyala
	while (i < count)
	{
		// Sadece değişken ismi eşleşirse ve '=' ile devam ediyorsa sil
		if (!(ft_strncmp(env[i], var, len) == 0 && env[i][len] == '='))
		{
			new_env[j++] = ft_strdup(env[i]); // eşleşmeyeni kopyala
		}
		// eşleşen satırı atla ve free etme (çünkü env malloc'lu değilse crash olur)
		i++;
	}
	new_env[j] = NULL;

	// 4. Eski env dizisini temizle
	free_env(env);

	return (new_env);
}

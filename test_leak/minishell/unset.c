/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:18:52 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/01 18:28:31 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**remove_from_env(char **env, const char *var)
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

void ft_unset(t_command *cmd)
{
	int i = 1;

	while (cmd->args[i])
	{
		if (is_valid(cmd->args[i])) // kontrol koyman iyi olur
			cmd->tmp->env = remove_from_env(cmd->tmp->env, cmd->args[i]);
		else
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		i++;
	}
}
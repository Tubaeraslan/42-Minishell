/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:17:14 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/01 18:18:06 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void print_declare_line(char *env)
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

static void print_env_sorted(char **envp)
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

void ft_export(t_command *cmd)
{
    int i = 1;

    if (!cmd->args[i]) // Eğer argüman yoksa env'yi sıralı göster
    {
        print_env_sorted(cmd->tmp->env);
        return;
    }

    while (cmd->args[i])
    {
        if (!is_valid(cmd->args[i]))
        {
            ft_putstr_fd("minishell: export: `", 2);
            ft_putstr_fd(cmd->args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
        }
        else
        {
            // Sadece '=' varsa ekle
            if (ft_strchr(cmd->args[i], '='))
                update_env(&cmd->tmp->env, cmd->args[i]);
            // '=' yoksa ekleme, sadece hata da yazma
        }
        i++;
    }
}

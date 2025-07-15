/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:17:14 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/02 14:42:05 by teraslan         ###   ########.fr       */
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
    int error_flag = 0;

    if (!cmd->args[i]) // argüman yoksa sıralı env göster
    {
        print_env_sorted(cmd->tmp->env);
        cmd->last_exit_code = 0; // başarı
        return;
    }

    while (cmd->args[i])
    {
        if (!is_valid(cmd->args[i]))
        {
            ft_putstr_fd("minishell: export: `", 2);
            ft_putstr_fd(cmd->args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            error_flag = 1;
        }
        else
        {
            if (ft_strchr(cmd->args[i], '='))
                update_env(&cmd->tmp->env, cmd->args[i]);
        }
        i++;
    }

    cmd->last_exit_code = error_flag ? 1 : 0;
}

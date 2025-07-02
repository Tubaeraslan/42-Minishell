/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:16:12 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/02 14:55:55 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int numeric_control(char *arg)
{
    int i = 0;

    if (!arg)
        return 0;

    // Başta + veya - olabilir, kabul et
    if (arg[i] == '+' || arg[i] == '-')
        i++;

    if (arg[i] == '\0') // Sadece + veya - varsa sayı değil
        return 1;

    while (arg[i])
    {
        if (!ft_isdigit(arg[i]))
            return 1; // sayı değil
        i++;
    }
    return 0; // sayı
}

static void exit_program(t_command *cmd, int exit_code)
{
    (void)cmd;
    // Bellek temizliği
    //all_free(cmd);  // t_command içindeki argümanlar, path, heredoc vs. hepsi free edilmeli

    // readline geçmişi varsa temizle
    rl_clear_history();

    // Diğer sistem kaynakları (örneğin dosya descriptorları) varsa kapatılmalı

    exit(exit_code);
}

int	ft_exit(t_command *cmd)
{
	long exit_code;

	write(2, "exit\n", 5);

	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return 1;
	}

	if (cmd->args[1] && numeric_control(cmd->args[1]) == 1)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit_program(cmd, 2);
	}

	if (cmd->args[1])
	{
		exit_code = ft_atoi(cmd->args[1]); // ft_atol, ft_atoi'nun long versiyonu
		if (exit_code < 0)
			exit_code = 256 + (exit_code % 256);
		exit_program(cmd, (int)(exit_code % 256));
	}
	else
		exit_program(cmd, 0);

	return 0;
}

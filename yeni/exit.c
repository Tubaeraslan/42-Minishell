/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:16:12 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/01 18:22:47 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int numeric_control(char *arg)
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
	write(2, "exit\n", 5);

	// Birden fazla argüman varsa, shell kapanmasın ama kod 1 dönsün
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1); // Shell devam eder, exit() çağrılmaz
	}

	// Argüman sayı değilse, shell 255 ile kapanmalı
	if (cmd->args[1] && numeric_control(cmd->args[1]) == 1)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit_program(cmd, 255);
	}

	// Geçerli sayısal argüman varsa
	else if (cmd->args[1])
		exit_program(cmd, ft_atoi(cmd->args[1]));
	else
		exit_program(cmd, 0);

	return (0); // Exit çağrıldıktan sonra burası çalışmaz ama syntax için
}

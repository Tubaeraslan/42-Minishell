/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:13:40 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/01 18:14:16 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_echo(t_command *cmd)
{
	int i;
	int newline;

	i = 1;
	if (cmd->args[1] && ft_strncmp(cmd->args[1],"-n",3) == 0)
	{
		newline = 0;
		i++;
	}
	
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i],1);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	
}

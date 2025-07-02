/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:15:36 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/01 18:15:47 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_env(t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd->tmp->env[i] != NULL)
	{
		j = 0;
		while (cmd->tmp->env[i][j] != '\0')
		{
			write(1, &cmd->tmp->env[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

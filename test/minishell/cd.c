/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:20:10 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/02 14:50:10 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_cd(t_command *cmd)
{
	int arg_count = 0;
	char *path;

	while (cmd->args[arg_count])
		arg_count++;

	if (arg_count > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		cmd->last_exit_code = 1;
		return;
	}

	if (!cmd->args[1] || strncmp(cmd->args[1], "~", 2) == 0)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			cmd->last_exit_code = 1;
			return;
		}
	}
	else
		path = cmd->args[1];

	if (chdir(path) != 0)
	{
		perror("cd");
		cmd->last_exit_code = 1;
		return;
	}

	cmd->last_exit_code = 0;
}
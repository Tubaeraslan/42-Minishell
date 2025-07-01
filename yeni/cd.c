/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:20:10 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/01 18:20:22 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_cd(t_command *cmd)
{
	char *path;

	if (!cmd->args[1] || strncmp(cmd->args[1], "~",2) == 0)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: HOME not set\n");
            return;
		}
	}
	else
		path = cmd->args[1];
		
	if (chdir(path) != 0)
	{
		perror("cd");
	}
	
}
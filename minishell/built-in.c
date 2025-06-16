/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:44:16 by teraslan          #+#    #+#             */
/*   Updated: 2025/06/16 19:16:47 by teraslan         ###   ########.fr       */
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

void ft_cd(t_command *cmd){
	
}

void ft_pwd(t_command *cmd)
{
	char *cwd;

	cwd = getcwd(NULL,0);
	if (cwd)
	{
		printf("%s\n",cwd);
		free(cwd);
	}
	else
		perror("pwd error");
}

void ft_export(t_command *cmd){
	
}

void ft_unset(t_command *cmd){
	
}

void ft_env(t_command *cmd){
	
}

void ft_exit(t_command *cmd){
	
}
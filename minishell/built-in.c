/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:44:16 by teraslan          #+#    #+#             */
/*   Updated: 2025/06/21 17:55:19 by ican             ###   ########.fr       */
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

void ft_cd(t_command *cmd)
{
	
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

void ft_export(t_command *cmd)
{
	 
}

void ft_unset(t_command *cmd)
{
	
}

void ft_env(t_command *cmd)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	if (cmd->args[1])
	{
		ft_putstr_fd(cmd->tmp->env,1);
	}
	while (cmd->tmp->env[i] != '\0')
	{
		while (cmd->tmp->env[i][j] != '\0')
		{
			write(1,&cmd->tmp->env[i][j],1);
			j++;
		}
		i++;
	}
}

void ft_exit(t_command *cmd)
{
	int	i;

	i = 0;
	write(2,"exit\n",5);
	while(i <= 0)
	{
		if (cmd->args[1] && cmd->args[2])
		{
				ft_putstr_fd("minishell: exit: too many arguments",STDERR_FILENO);
				i++;
		}
		if (cmd->args[1] && 1 == numeric_control(cmd->args[1]))
		{
			write(2, "minishell: exit: ", 18);
			ft_putstr_fd(cmd->args[1], 2);
			write(2, ": numeric argument required", 28);
			exit_program(cmd, -1);
		}
		else if(cmd->args[1])
			exit_program(cmd, ft_atol(cmd->args[1]));//
		else
			exit_program(cmd, 0);	
	}
}
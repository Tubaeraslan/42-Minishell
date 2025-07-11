/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:20:10 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/11 18:45:36 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_cd(t_command *cmd)
{
	int arg_count = 0;
	char *path;
	char cwd[1024];
	char *oldpwd=NULL;

	while (cmd->args[arg_count])
		arg_count++;

	if (arg_count > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		cmd->last_exit_code = 1;
		return;
	}
	//oldpwd
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		cmd->last_exit_code = 1;
		return;
	}
	oldpwd = ft_strdup(cwd);
	if (!oldpwd)
	{
		perror("strdup");
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
			free(oldpwd);
			return;
		}
	}
	else
		path = cmd->args[1];

	if (chdir(path) != 0)
	{
		perror("cd");
		cmd->last_exit_code = 1;
		free(oldpwd);
		return;
	}
	// Yeni path'i al
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		cmd->last_exit_code = 1;
		free(oldpwd);
		return;
	}
	char *oldpwd_var = ft_strjoin("OLDPWD=", oldpwd);
	char *pwd_var = ft_strjoin("PWD=", cwd);
	if (!oldpwd_var || !pwd_var)
	{
		free(oldpwd_var);
		free(pwd_var);
		free(oldpwd);
		cmd->last_exit_code = 1;
		return;
	}
	update_env(&cmd->tmp->env, oldpwd_var);
	update_env(&cmd->tmp->env, pwd_var);

	free(oldpwd_var);
	free(pwd_var);
	free(oldpwd);
	cmd->last_exit_code = 0;
}

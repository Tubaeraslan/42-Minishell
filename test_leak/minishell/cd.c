/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:20:10 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/16 14:19:34 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_prepare(t_command *cmd, char *cwd, char **oldpwd, char **path)
{
	size_t	cwd_size;

	cwd_size = 1024;
	if (get_oldpwd(cmd, cwd, cwd_size, oldpwd))
		return (1);
	if (get_path(cmd, path))
		return (1);
	return (0);
}

static int	cd_change_dir(t_command *cmd, char *path, char **newcwd)
{
	char	cwd[1024];

	if (chdir(path) != 0)
	{
		perror("cd");
		cmd->last_exit_code = 1;
		return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		cmd->last_exit_code = 1;
		return (1);
	}
	*newcwd = ft_strdup(cwd);
	if (!*newcwd)
	{
		perror("strdup");
		cmd->last_exit_code = 1;
		return (1);
	}
	return (0);
}

static int	update_pwd_oldpwd_env(t_command *cmd, char *oldpwd, char *newcwd)
{
	char	*oldpwd_var;
	char	*pwd_var;

	oldpwd_var = ft_strjoin("OLDPWD=", oldpwd);
	pwd_var = ft_strjoin("PWD=", newcwd);
	if (!oldpwd_var || !pwd_var)
	{
		free(oldpwd_var);
		free(pwd_var);
		cmd->last_exit_code = 1;
		return (1);
	}
	update_env(&cmd->tmp->env, oldpwd_var);
	update_env(&cmd->tmp->env, pwd_var);
	free(oldpwd_var);
	free(pwd_var);
	return (0);
}

static int	cd_chdir_and_update_env(t_command *cmd, char *path, char *oldpwd)
{
	char	*newcwd;

	if (cd_change_dir(cmd, path, &newcwd))
	{
		free(oldpwd);
		return (1);
	}
	if (update_pwd_oldpwd_env(cmd, oldpwd, newcwd))
	{
		free(oldpwd);
		free(newcwd);
		return (1);
	}
	free(oldpwd);
	free(newcwd);
	return (0);
}

void	ft_cd(t_command *cmd)
{
	int		arg_count;
	char	*path;
	char	cwd[1024];
	char	*oldpwd;

	oldpwd = NULL;
	arg_count = 0;
	while (cmd->args[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		cmd->last_exit_code = 1;
		return ;
	}
	if (cd_prepare(cmd, cwd, &oldpwd, &path))
		return ;
	if (cd_chdir_and_update_env(cmd, path, oldpwd))
		return ;
	cmd->last_exit_code = 0;
}

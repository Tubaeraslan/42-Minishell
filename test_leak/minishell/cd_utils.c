/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:11:44 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/16 14:18:13 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_oldpwd(t_command *cmd, char *cwd, size_t c_size, char **oldpwd)
{
	if (!getcwd(cwd, c_size))
	{
		perror("getcwd");
		cmd->last_exit_code = 1;
		return (1);
	}
	*oldpwd = ft_strdup(cwd);
	if (!*oldpwd)
	{
		perror("strdup");
		cmd->last_exit_code = 1;
		return (1);
	}
	return (0);
}

int	get_path(t_command *cmd, char **path)
{
	if (!cmd->args[1] || strncmp(cmd->args[1], "~", 2) == 0)
	{
		*path = getenv("HOME");
		if (!*path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			cmd->last_exit_code = 1;
			return (1);
		}
	}
	else
		*path = cmd->args[1];
	return (0);
}

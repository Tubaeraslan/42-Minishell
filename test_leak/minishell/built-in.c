/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:06:18 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/01 18:39:54 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid(char *str)
{
	int	i;

	i = 0;
	if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	execute_builtin_with_redir(t_command *command)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	handle_redirections(command);
	if (ft_strncmp(command->cmd, "exit", 5) == 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		command->last_exit_code = ft_exit(command);
		return ;
	}
	execute_built(command);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

char	*get_basename(char *path)
{
	char	*slash;

	if (!path)
		return (NULL);
	slash = ft_strrchr(path, '/');
	if (slash)
		return (slash + 1);
	return (path);
}

int	is_built(char *arg)
{
	char	*cmd;
	int		len;

	if (!arg)
		return (0);
	cmd = arg;
	len = ft_strlen(arg);
	while (len > 0 && arg[len] != '/')
		len--;
	if (arg[len] == '/')
		cmd = &arg[len + 1];
	if (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

void	execute_built(t_command *cmd)
{
	char	*cmd_name;
	int		len;

	if (!cmd || !cmd->cmd)
		return ;
	cmd_name = cmd->cmd;
	len = ft_strlen(cmd->cmd);
	while (len > 0 && cmd->cmd[len] != '/')
		len--;
	if (cmd->cmd[len] == '/')
		cmd_name = &cmd->cmd[len + 1];
	if (ft_strncmp(cmd_name, "echo", 5) == 0)
		ft_echo(cmd);
	else if (ft_strncmp(cmd_name, "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(cmd_name, "env", 4) == 0)
		ft_env(cmd);
	else if (ft_strncmp(cmd_name, "exit", 5) == 0)
		cmd->last_exit_code = ft_exit(cmd);
	else if (ft_strncmp(cmd_name, "export", 7) == 0)
		ft_export(cmd);
	else if (ft_strncmp(cmd_name, "unset", 6) == 0)
		ft_unset(cmd);
	else if (ft_strncmp(cmd_name, "cd", 3) == 0)
		ft_cd(cmd);
}

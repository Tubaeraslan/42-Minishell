/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:06:18 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/01 18:33:34 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_valid(char *str)
{
	int i = 0;

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
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);

	handle_redirections(command);
	execute_built(command);

	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

int	is_built(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strncmp(arg, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(arg, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(arg, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(arg, "export", 7) == 0)
		return (1);
	if (ft_strncmp(arg, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(arg, "env", 4) == 0)
		return (1);
	if (ft_strncmp(arg, "exit", 5) == 0)
		return (1);
	return (0);
}

void execute_built(t_command *cmd)
{	
    if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
    {
        ft_echo(cmd);
        cmd->last_exit_code = 0;
    }
    else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
    {
        ft_pwd();
        cmd->last_exit_code = 0;
    }
    else if (ft_strncmp(cmd->cmd, "env", 4) == 0)
    {
        ft_env(cmd);
        cmd->last_exit_code = 0;
    }
    else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
    {
        cmd->last_exit_code = ft_exit(cmd);
    }
	else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
    {
        ft_export(cmd);
		cmd->last_exit_code = 0;
    }
	else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
    {
        ft_unset(cmd);
		cmd->last_exit_code = 0;
    }
	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
    {
        ft_cd(cmd);
		cmd->last_exit_code = 0;
    }
}


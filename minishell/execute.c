/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:09:19 by ican              #+#    #+#             */
/*   Updated: 2025/06/28 15:38:27 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

//run built-in
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

char	*path_finder(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	char	*temp;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':'); // "PATH=" sonrası
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path, X_OK) == 0)
		{
			//ft_free_split(paths);
			return (path);
		}
		free(path);
		i++;
	}
	//ft_free_split(paths);
	return (NULL);
}

//pipe yoksa burda çalışıcak
void	execute_a_token(t_command *command)
{
	pid_t	pid;
	char	*path;
	int		status;
	int		fd;

	if (!command || !command->cmd)
		return ;

	if (is_built(command->cmd) == 1)
	{
		execute_built(command);
		return ;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) // child process
	{
		// 🔄 INFILE redirect
		if (command->infile)
		{
			fd = open(command->infile, O_RDONLY);
			if (fd < 0)
			{
				perror("open infile");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}

		// 🔄 OUTFILE redirect
		if (command->outfile)
		{
			if (command->is_append)
				fd = open(command->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd = open(command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

			if (fd < 0)
			{
				perror("open outfile");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}

		path = path_finder(command->cmd, command->tmp->env);
		if (!path)
		{
			ft_putstr_fd("command not found: ", 2);
			ft_putstr_fd(command->cmd, 2);
			ft_putchar_fd('\n', 2);
			exit(127);
		}
		execve(path, command->args, command->tmp->env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else // parent process
		waitpid(pid, &status, 0);
}



void	execute_commands(t_command *command)
{
	if (!command || command->token_count == 0)
		return ;	
	else if (!command->is_pipe)
		execute_a_token(command);
	// else
	// 	execute_many_token(command);
}

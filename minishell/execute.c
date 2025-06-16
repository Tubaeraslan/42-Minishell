/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:09:19 by ican              #+#    #+#             */
/*   Updated: 2025/06/16 19:18:22 by teraslan         ###   ########.fr       */
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
void	execute_built(t_command *cmd)
{
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
	 	ft_echo(cmd);
	// else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
	// 	ft_cd(cmd);
	else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
	 	ft_pwd(cmd);
	// else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
	// 	ft_export(cmd);
	// else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
	// 	ft_unset(cmd);
	// else if (ft_strncmp(cmd->cmd, "env", 4) == 0)
	// 	ft_env(cmd);
	// else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
	// 	ft_exit(cmd);
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
	
	if (!command || !command->cmd)
		return ; 
	if (is_built(command->cmd) == 1)
	{
		execute_built(command);
		return ;
	}
	pid = fork();
	printf("%d\n",pid);
	//fork hatası
	if (pid == -1)
	{
		perror("fork");
		exit(-1);
	}

	//child process
	else if (pid == 0)
	{
		printf("%d\n",pid);
		path = path_finder(command->cmd,command->tmp->env); //komutun path değişkeninindeki yerini bul
		//no path ifff
		if (!path)
		{
			ft_putstr_fd("command not found: ", 2);
			ft_putstr_fd(command->cmd, 2);
			ft_putchar_fd('\n', 2);
			exit(127);
		}
		execve(path,command->args,command->tmp->env);
		perror("execve");
		exit(-1);
	}
	
	//parent process
	else
	{
		printf("%d\n",pid);
		waitpid(pid, &status, 0);
	}
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:09:19 by ican              #+#    #+#             */
/*   Updated: 2025/06/28 17:52:46 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

void setup_heredoc(t_command *cmd)
{
    int pipe_fd[2];
    char *line;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        line = readline("> ");
        if (!line)
            break;

        // Limiter ile eşleşiyorsa çık
        if (strcmp(line, cmd->heredoc_limiter) == 0)
        {
            free(line);
            break;
        }

        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }

    close(pipe_fd[1]);
    cmd->heredoc_fd = pipe_fd[0];
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

void	handle_redirections(t_command *cmd)
{
	int	fd;

	// Eğer heredoc varsa infile yerine heredoc öncelikli olsun
	if (cmd->is_heredoc)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	else if (cmd->infile) // heredoc yoksa infile aç
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
		{
			perror("open infile");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}

	if (cmd->outfile)
	{
		if (cmd->is_append)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open outfile");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}


//pipe yoksa burda çalışıcak
void	execute_a_token(t_command *command)
{
	pid_t	pid;
	char	*path;
	int		status;

	if (!command || !command->cmd)
		return ;

	if (command->is_heredoc)
		setup_heredoc(command);

	if (is_built(command->cmd) == 1)
	{
		int saved_stdin = dup(STDIN_FILENO);
		int saved_stdout = dup(STDOUT_FILENO);

		handle_redirections(command);     // yönlendirmeyi yap
		execute_built(command);           // built-in komutu çalıştır

		// stdout ve stdin'i geri döndür
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
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
		handle_redirections(command);

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

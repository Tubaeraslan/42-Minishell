/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:34:23 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/19 13:05:27 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*path_finder(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	char	*temp;
	int		i;

	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path, X_OK) == 0)
		{
			return (path);
		}
		free(path);
		i++;
	}
	free_two_dimension(paths);
	return (NULL);
}

void execute_child_process(t_command *command)
{
	char *path;

	if (handle_redirections(command) == -1)
		exit(1);
	path = path_finder(command->cmd, command->tmp->env);
	if (!path)
	{
		ft_putstr_fd(command->cmd, 2);
		ft_putstr_fd(": command not found", 2);
		ft_putchar_fd('\n', 2);
		exit(127);
	}
	execve(path, command->args, command->tmp->env);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	handle_fork_error(void)
{
	perror("fork");
	exit(EXIT_FAILURE);
}

void reset_pipe_flags(t_command *command)
{
    while (command)
    {
        command->is_pipe = 0;
        command = command->next;
    }
}
void free_command_fields(t_command *command)
{
    if (command->cmd)
    {
        free(command->cmd);
        command->cmd = NULL;
    }
    // Eğer varsa args, tokens, redirect vs. de temizlenmeli:
    // free_args(command->args);
    // free_tokens(command->tokens);
    // free_redirects(command->redirects);
    // vs.
}

void execute_commands(t_command *command)
{
    if (!command || command->token_count == 0)
        return;

    if (!command->is_pipe)
    {
        if (!command->parsing_error)
            execute_a_token(command);
    }
    else
    {
        command->last_exit_code = execute_many_token(command);
    }
    reset_pipe_flags(command);
    free_command_fields(command);
}

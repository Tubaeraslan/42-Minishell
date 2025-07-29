/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:26:56 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/29 12:52:25 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

char	*get_command_path(t_command *cmd)
{
	char	*path;

	if (cmd->cmd[0] == '/' || (cmd->cmd[0] == '.' && cmd->cmd[1] == '/'))
	{
		path = ft_strdup(cmd->cmd);
		if (!path)
		{
			perror("ft_strdup");
			exit(1);
		}
	}
	else
	{
		path = path_finder(cmd->cmd, cmd->tmp->env);
		if (!path)
		{
			ft_putstr_fd("command not found: ", 2);
			ft_putstr_fd(cmd->cmd, 2);
			ft_putchar_fd('\n', 2);
			exit(127);
		}
	}
	return (path);
}

static void	print_err_and_exit(char *path, char *msg, int code, t_command *cmd)
{
	ft_putstr_fd(path, 2);
	ft_putstr_fd(msg, 2);
	free(path);
	all_free(cmd);
	exit(code);
}

void	check_path_validity(char *path, t_command *cmd)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		print_err_and_exit(path, ": No such file or directory\n", 127, cmd);
	if (S_ISDIR(st.st_mode))
		print_err_and_exit(path, ": Is a directory\n", 126, cmd);
	if (!(st.st_mode & S_IXUSR))
		print_err_and_exit(path, ": Permission denied\n", 126, cmd);
}

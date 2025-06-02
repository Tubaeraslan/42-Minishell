/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <ican@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:09:19 by ican              #+#    #+#             */
/*   Updated: 2025/06/02 16:59:03 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_built(char *arg)
{
	return (ft_strcmp(arg, "echo") || ft_strcmp(arg, "cd") || ft_strcmp(arg, "pwd") || ft_strcmp(arg, "export") 
	|| ft_strcmp(arg, "unset") || ft_strcmp(arg, "env") || ft_strcmp(arg, "exit"));

}

void	execute_buiild()
{
	
}

void	execute_a_token(t_command *command)
{
	pid_t	pid;
	char	*path;
	int	a;

	if (!command || command->cmd)
		return ; 
	if (is_built(command->cmd) == 0)
	{
		execute_built(command);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(-1);
	}
	else if (pid == 0)
	{
		path = path_finder();
		//no path ifff
		execve(path,command->cmd,command->tmp->env);
		
	}
	
		
}


void	execute_commands(t_command *command)
{
	if (!command || command->token_count == 0)
		return ;	
	else if (command->token_count == 1)
		execute_a_token(command);
	else
		execute_many_token(command);
}

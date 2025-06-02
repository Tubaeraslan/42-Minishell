/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <ican@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:09:19 by ican              #+#    #+#             */
/*   Updated: 2025/06/02 11:56:36 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_a_command(t_command *command)
{
	
}


void	execute_commands(t_command *command)
{
	if(command->token_count == 1)
		execute_a_command(&command);
	else if(command->token_count >= 1)
		execute_many_command(&command);
}

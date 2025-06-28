/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_many.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:52:32 by ican              #+#    #+#             */
/*   Updated: 2025/06/28 16:41:42 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    execute_many_token(t_command	*command)
{
	int	out;
	int fd[2];
	int size;

	ft_pipe_start();
	ft_pipe_close();
	

}
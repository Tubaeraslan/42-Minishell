/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:14:33 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/01 18:14:54 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_pwd()
{
	char *cwd;

	cwd = getcwd(NULL,0);
	if (cwd)
	{
		printf("%s\n",cwd);
		free(cwd);
	}
	else
		perror("pwd error");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:03:05 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/29 17:11:29 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirects(t_redirects *redir)
{
	t_redirects	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->name)
		{
			free(redir->name);
			redir->name = NULL;
		}
		free(redir);
		redir = tmp;
	}
}

void	free_heredocs(t_heredoc *heredoc)
{
	t_heredoc	*tmp;

	while (heredoc)
	{
		tmp = heredoc->next;
		if (heredoc->limiter)
		{
			free(heredoc->limiter);
			heredoc->limiter = NULL;
		}
		free(heredoc);
		heredoc = tmp;
	}
}

void free_command_list_except_first(t_command *cmd)
{
    t_command *tmp;
    t_command *next_cmd;

    if (!cmd || !cmd->next)
        return;
    tmp = cmd->next;
    while (tmp)
    {
        free_command_fields(tmp);  // iç alanları boşalt
		next_cmd = tmp->next;
        free(tmp);                 // struct'ı serbest bırak
        tmp = next_cmd;
    }
    cmd->next = NULL; // liste sonlandırılıyor
}

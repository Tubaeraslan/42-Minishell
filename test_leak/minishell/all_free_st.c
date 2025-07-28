/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free_st.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:09:21 by ican              #+#    #+#             */
/*   Updated: 2025/07/28 16:39:11 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_two_dimension(char **arg)
{
    int i;

    i = 0;
    if (!arg)
        return ;
    while (arg[i])
    {
        if (arg[i])
        {
            free(arg[i]);
            arg[i] = NULL;  
        }
        i++;
    }
    free(arg);
    arg = NULL;
}
void    free_data(t_data *tmp)
{
    if (!tmp)
        return;
    free_two_dimension(tmp->env);
    free(tmp->input);
        tmp->input = NULL;
    free(tmp);
    tmp = NULL;
}


void all_free(t_command *cmd)
{
    if (!cmd)
        return;
    if ((cmd->tokens && cmd->tokens[0]) || cmd->tokens != NULL)
    {
        clear_tokens(cmd);
        cmd->tokens = NULL;
    }
    if (cmd->cmd && ft_strlen(cmd->cmd) > 0)
    {
        free(cmd->cmd);
        cmd->cmd = NULL;
    }
    if (cmd->args && cmd->args[0])
    {
        free_two_dimension(cmd->args);
        cmd->args = NULL;
    }
    clear_command_data(cmd);
    free_data(cmd->tmp);
    cmd->tmp = NULL;
    free(cmd);
    cmd = NULL;
}

//parsing için lazım
void clear_command_data(t_command *cmd)
{
	if (cmd->infile)
	{
		free(cmd->infile);
		cmd->infile = NULL;
	}
	if (cmd->outfile)
	{
		free(cmd->outfile);
		cmd->outfile = NULL;
	}
	if (cmd->heredoc_limiter)
    {
        free(cmd->heredoc_limiter);
        cmd->heredoc_limiter = NULL;
    }
	cmd->is_pipe = 0;
	cmd->is_append = 0;
    cmd->error_printed = 0;
	cmd->parsing_error = 0;
}
//parsing için
void	free_old_tokens(t_command *command)
{
	int	j;

	if (command->tokens)
	{
		j = 0;
		while (j < command->token_count)
		{
			free(command->tokens[j]);
			command->tokens[j] = NULL;
			j++;
		}
		free(command->tokens);
		command->tokens = NULL;
	}
}

void free_redirects(t_redirects *redir)
{
    t_redirects *tmp;

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

void free_heredocs(t_heredoc *heredoc)
{
    t_heredoc *tmp;

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

//execute.c için
void free_command_fields(t_command *cmd)
{
    if (!cmd)
        return;
    if (cmd->cmd)
    {
        free(cmd->cmd);
        cmd->cmd = NULL;
    }
    if (cmd->args)
    {
        free_two_dimension(cmd->args);
        cmd->args = NULL;
    }
    if (cmd->tokens)
    {
        clear_tokens(cmd);
    }
    if (cmd->infile)
    {
        free(cmd->infile);
        cmd->infile = NULL;
    }
    if (cmd->outfile)
    {
        free(cmd->outfile);
        cmd->outfile = NULL;
    }
    if (cmd->heredoc_limiter)
    {
        free(cmd->heredoc_limiter);
        cmd->heredoc_limiter = NULL;
    }
    if (cmd->redirects)
    {
        free_redirects(cmd->redirects);
        cmd->redirects = NULL;
    }
    if (cmd->heredocs)
    {
        free_heredocs(cmd->heredocs);
        cmd->heredocs = NULL;
    }
    if (cmd->export_list)
    {
        free_two_dimension(cmd->export_list);
        cmd->export_list = NULL;
    }
}
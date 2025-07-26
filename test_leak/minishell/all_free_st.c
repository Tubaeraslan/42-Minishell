/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free_st.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:09:21 by ican              #+#    #+#             */
/*   Updated: 2025/07/26 18:53:44 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_two_dimension(char **arg)
{
    int i;

    i = 0;
    if (!arg || !*arg)
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
}
void    free_data(t_data *tmp)
{
    if (!tmp)
        return;
    free_two_dimension(tmp->env);
    free(tmp->input);
        tmp->input = NULL;
}


void all_free(t_command *cmd)
{
    if (!cmd)
        return;
    //if ((comd->tokens && comd->tokens[0]) || comd->tokens != NULL)
      //  clear_tokens(comd);
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

//execute.c için
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
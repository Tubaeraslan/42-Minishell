/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free_st.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:09:21 by ican              #+#    #+#             */
/*   Updated: 2025/06/21 16:59:31 by ican             ###   ########.fr       */
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
        free(arg[i]);
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
    free(tmp);
}


void    all_free(t_command *cmd)
{
    if (!cmd)
        return;
    free_two_dimension(cmd->tokens);
    free(cmd->cmd);
    free_two_dimension(cmd->args);
    free(cmd->infile);
    free(cmd->outfile);
    free_data(cmd->tmp);
    free(cmd->next);
    free(cmd);
}

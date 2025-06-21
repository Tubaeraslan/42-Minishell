/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:43:03 by ican              #+#    #+#             */
/*   Updated: 2025/06/21 17:19:30 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int numeric_control(char *arg)
{
    int i;

    i = 0;
    if(!arg)
        return (0);

    while (arg[i] != '\0')
    {
        if(ft_isdigit(arg[i]) == 0)
            return (1);
        i++;
    }
    return(0);
}

void exit_program(t_command *cmd, int exit_code)
{
    all_free(cmd);
    exit(exit_code);

}
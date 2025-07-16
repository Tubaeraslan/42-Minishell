/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 19:24:49 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/16 14:41:35 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void expand_variable_from_env(t_expand *ex, t_command *command)
{
    ex->i++;
    char varname[256];
    int k = 0;
	
    if (ex->input[ex->i] == '?')
    {
        char *exit_str = ft_itoa(command->last_exit_code);
        int k = 0;
        while (exit_str[k])
            ex->new_input[ex->j++] = exit_str[k++];
        free(exit_str);
        ex->i++; // '?' karakterini atla
        return;
    }
    while (ex->input[ex->i] && (ft_isalnum(ex->input[ex->i]) || ex->input[ex->i] == '_'))
        varname[k++] = ex->input[ex->i++];
    varname[k] = '\0';

    if (k == 0)
    {
        // Değişken adı yok, yani sadece '$' var
        // Bu durumda '$' karakterini olduğu gibi ekle
        ex->new_input[ex->j++] = '$';
        return;
    }

    char *value = get_env_value(command->tmp->env, varname);
    if (value)
    {
        int m = 0;
        while (value[m])
            ex->new_input[ex->j++] = value[m++];
    }
}

void expand_loop(t_expand *ex, t_command *command)
{
	while (ex->input[ex->i])
	{
		if (ex->input[ex->i] == '\'' && !ex->double_quote)
		{
			ex->single_quote = !ex->single_quote;
			ex->new_input[ex->j++] = ex->input[ex->i++];
		}
		else if (ex->input[ex->i] == '"' && !ex->single_quote)
		{
			ex->double_quote = !ex->double_quote;
			ex->new_input[ex->j++] = ex->input[ex->i++];
		}
		else if (ex->input[ex->i] == '$' && !ex->single_quote)
			expand_variable_from_env(ex, command);
		else
			ex->new_input[ex->j++] = ex->input[ex->i++];
	}
}

void expand_variables(t_command *command)
{
	t_expand *ex;

	ex = malloc(sizeof(t_expand));
	if (!ex)
		return;
	ex->input = command->tmp->input;
	ex->new_input = malloc(4096);
	if (!ex->new_input)
	{
		free(ex);
		return;
	}
	ex->i = 0;
	ex->j = 0;
	ex->single_quote = 0;
	ex->double_quote = 0;

	expand_loop(ex, command);

	ex->new_input[ex->j] = '\0';
	free(command->tmp->input);
	command->tmp->input = ft_strdup(ex->new_input);
	free(ex->new_input);
	free(ex);
}

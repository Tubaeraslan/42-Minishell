/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 19:24:49 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/19 13:22:16 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_exit_code(t_expand *ex, t_command *command)
{
	char	*exit_str;
	int		k;

	exit_str = ft_itoa(command->last_exit_code);
	k = 0;
	while (exit_str[k])
		ex->new_input[ex->j++] = exit_str[k++];
	free(exit_str);
	ex->i++;
}

static void	copy_env_value_to_input(t_expand *ex, char *value)
{
	int	m;

	m = 0;
	while (value[m])
		ex->new_input[ex->j++] = value[m++];
}

void	expand_variable_from_env(t_expand *ex, t_command *command)
{
	char	varname[256];
	int		k;
	char	*value;

	k = 0;
	ex->i++;
	if (ex->input[ex->i] == '?')
	{
		expand_exit_code(ex, command);
		return ;
	}
	while (ex->input[ex->i]
		&& (ft_isalnum(ex->input[ex->i]) || ex->input[ex->i] == '_'))
		varname[k++] = ex->input[ex->i++];
	varname[k] = '\0';
	if (k == 0)
	{
		ex->new_input[ex->j++] = '$';
		return ;
	}
	value = get_env_value(command->tmp->env, varname);
	if (value)
		copy_env_value_to_input(ex, value);
}

void	expand_loop(t_expand *ex, t_command *command)
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

void	expand_variables(t_command *command)
{
	t_expand	*ex;

	ex = malloc(sizeof(t_expand));
	if (!ex)
		return ;
	ex->input = command->tmp->input;
	ex->new_input = malloc(4096);
	if (!ex->new_input)
	{
		free(ex);
		return ;
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

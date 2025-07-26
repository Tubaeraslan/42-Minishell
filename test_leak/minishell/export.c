/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:17:14 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/26 12:33:20 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_in_env(char **env, char *var)
{
	int		i;
	size_t	var_len;

	var_len = strlen(var);
	i = 0;
	while (env[i])
	{
		if (strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
			return (1);
		i++;
	}
	return (0);
}

static int	is_in_list(char **list, char *var)
{
	int i = 0;

	while (list && list[i])
	{
		if (strcmp(list[i], var) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	update_export(char ***export_list, char *var)
{
	int		i;
	char	**new_list;

	if (is_in_list(*export_list, var))
		return ;
	i = 0;
	while (*export_list && (*export_list)[i])
		i++;
	new_list = malloc(sizeof(char *) * (i + 2));
	if (!new_list)
		return ;
	i = 0;
	while (*export_list && (*export_list)[i])
	{
		new_list[i] = strdup((*export_list)[i]);
		i++;
	}
	new_list[i++] = strdup(var);
	new_list[i] = NULL;
	if (*export_list)
	{
		int j = 0;
		while ((*export_list)[j])
			free((*export_list)[j++]);
		free(*export_list);
	}
	*export_list = new_list;
}

static void	print_declare_line(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		write(1, &env[i++], 1);
	if (env[i] == '=')
	{
		write(1, "=\"", 2);
		ft_putstr_fd(&env[i + 1], 1);
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

static void	print_export_list(char **env, char **export_list)
{
	char **sorted_env = sort_env(env);
	char **sorted_export = sort_env(export_list);
	int i = 0;
	while (sorted_env[i])
	{
		ft_putstr_fd("declare -x ", 1);
		print_declare_line(sorted_env[i]);
		i++;
	}
	i = 0;
	while (sorted_export[i])
	{
		if (!find_in_env(sorted_env, sorted_export[i]))
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(sorted_export[i], 1);
			write(1, "\n", 1);
		}
		i++;
	}
	free_env(sorted_env);
	free_env(sorted_export);
}

static int	ft_export_handle_arg(t_command *cmd, char *arg)
{
	if (!is_valid(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (ft_strchr(arg, '='))
		update_env(&cmd->tmp->env, arg);
	else
		update_export(&cmd->export_list, arg);
	return (0);
}

void	ft_export(t_command *cmd)
{
	int		i;
	int		error_flag;

	i = 1;
	error_flag = 0;
	if (!cmd->args[i])
	{
		print_export_list(cmd->tmp->env, cmd->export_list);
		cmd->last_exit_code = 0;
		return ;
	}
	while (cmd->args[i])
	{
		if (ft_export_handle_arg(cmd, cmd->args[i]))
			error_flag = 1;
		i++;
	}
	if (error_flag)
		cmd->last_exit_code = 1;
	else
		cmd->last_exit_code = 0;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:17:14 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/16 14:46:30 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	print_env_sorted(char **envp)
{
	char	**sorted;
	int		i;

	sorted = sort_env(envp);
	i = 0;
	while (sorted[i])
	{
		ft_putstr_fd("declare -x ", 1);
		print_declare_line(sorted[i]);
		i++;
	}
	free_env(sorted);
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
		print_env_sorted(cmd->tmp->env);
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

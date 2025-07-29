/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:31:17 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/29 13:50:05 by teraslan         ###   ########.fr       */
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

int	is_in_list(char **list, char *var)
{
	int	i;

	i = 0;
	while (list && list[i])
	{
		if (strcmp(list[i], var) == 0)
			return (1);
		i++;
	}
	return (0);
}

static char	**extend_export_list(char **export_list, char *var)
{
	int		i;
	char	**new_list;

	i = 0;
	while (export_list && export_list[i])
		i++;
	new_list = malloc(sizeof(char *) * (i + 2));
	if (!new_list)
		return (NULL);
	i = 0;
	while (export_list && export_list[i])
	{
		new_list[i] = strdup(export_list[i]);
		i++;
	}
	new_list[i++] = strdup(var);
	new_list[i] = NULL;
	return (new_list);
}

void	update_export(char ***export_list, char *var)
{
	char	**new_list;
	int		j;

	if (is_in_list(*export_list, var))
		return ;
	new_list = extend_export_list(*export_list, var);
	if (!new_list)
		return ;
	if (*export_list)
	{
		j = 0;
		while ((*export_list)[j])
			free((*export_list)[j++]);
		free(*export_list);
	}
	*export_list = new_list;
}

void	print_declare_line(char *env)
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

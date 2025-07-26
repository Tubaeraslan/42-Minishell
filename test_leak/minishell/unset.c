/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:18:52 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/26 12:38:10 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**remove_from_env(char **env, const char *var)
{
	int		i;
	int		j;
	int		count;
	char	**new_env;
	int		len;

	i = 0;
	j = 0;
	count = 0;
	len = ft_strlen(var);
	while (env && env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	while (i < count)
	{
		if (!(ft_strncmp(env[i], var, len) == 0 && env[i][len] == '='))
			new_env[j++] = ft_strdup(env[i]);
		i++;
	}
	new_env[j] = NULL;
	free_env(env);
	return (new_env);
}

void	ft_unset(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (is_valid(cmd->args[i]))
			cmd->tmp->env = remove_from_env(cmd->tmp->env, cmd->args[i]);
		i++;
	}
}

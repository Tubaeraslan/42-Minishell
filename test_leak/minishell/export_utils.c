/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:31:17 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/30 19:21:17 by teraslan         ###   ########.fr       */
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

int is_in_list(char **list, char *var)
{
    int i = 0;

    if (!var)
        return 0;
    if (!list)
        return 0;

    while (list[i])
    {
        if (!list[i])
        {
            break;
        }
        
        if (strcmp(list[i], var) == 0)
            return 1;
        i++;
    }
    return 0;
}

void extend_export_list(t_command *command, char *var)
{
    int i = 0;
    char **new_list;

    while (command->tmp->export_list && command->tmp->export_list[i])
        i++;

    new_list = malloc(sizeof(char *) * (i + 2));
    if (!new_list)
    {
        free(var);
        return;
    }

    // Eski pointer'ları kopyala (string'leri değil!)
    for (int j = 0; j < i; j++)
        new_list[j] = command->tmp->export_list[j];

    new_list[i] = var;
    new_list[i + 1] = NULL;

    // Sadece array'i free et, içindekileri değil
    if (command->tmp->export_list)
        free(command->tmp->export_list);
    
    command->tmp->export_list = new_list;
}

void update_export(t_command *command, char *var)
{
    if (is_in_list(command->tmp->export_list, var))
    {
        free(var);
        return;
    }
    extend_export_list(command, var);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:31:17 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/30 17:53:38 by teraslan         ###   ########.fr       */
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
            // Bu olmaz ama güvenlik için break
            break;
        }
        // Debug print:
        printf("Comparing list[%d] = \"%s\" with var = \"%s\"\n", i, list[i], var);
        
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

    for (int j = 0; j < i; j++)
    {
        new_list[j] = ft_strdup(command->tmp->export_list[j]);
        if (!new_list[j])
        {
            while (--j >= 0)
                free(new_list[j]);
            free(new_list);
            free(var);
            return;
        }
    }

    new_list[i] = var;  // var zaten strdup edilmiş olmalı
    new_list[i + 1] = NULL;

    if (command->tmp->export_list)
    {
        for (int k = 0; (command->tmp->export_list)[k]; k++)
            free((command->tmp->export_list)[k]);
        free(command->tmp->export_list);
    }
	char ** tmp = new_list;
    command->tmp->export_list = tmp;
	free(new_list);
	free(var);
	// free_two_dimension(new_list);
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

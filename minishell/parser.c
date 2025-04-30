/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:11:11 by teraslan          #+#    #+#             */
/*   Updated: 2025/04/30 17:41:01 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_input(t_data *shell)
{
	t_command *command;
	char **tokens;
	int i;

	i = 0;
	//boşsa işlem yapma
	if (!shell->input || shell->input[0] == '\0')
		return;
	//1) input u boşluklara göre ayır
}
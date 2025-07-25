/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:39:56 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/25 16:57:41 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (src[i] && i < len)
	{
		dst[i] = src[i];
		i++;
	}
	while (i < len)
	{
		dst[i] = '\0';
		i++;
	}
	return (dst);
}

void	copy_env_value_to_input(t_expand *ex, char *value)
{
	int	m;

	m = 0;
	while (value[m])
		ex->new_input[ex->j++] = value[m++];
}

void	expand_pid(t_expand *ex)
{
	char	*pid_str;
	int		m;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return ;
	m = 0;
	while (pid_str[m])
		ex->new_input[ex->j++] = pid_str[m++];
	free(pid_str);
	ex->i++;
}

int	read_varname(char *input, int *i, char *varname)
{
	int	k;

	k = 0;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		varname[k++] = input[*i];
		(*i)++;
	}
	varname[k] = '\0';
	return (k);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 12:29:35 by teraslan          #+#    #+#             */
/*   Updated: 2024/10/26 11:40:29 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	ft_word_count(const char *s, char c)
{
	int	len;

	len = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			len++;
		while (*s != c && *s)
			s++;
	}
	return (len);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		len;
	int		j;
	char	**result;

	i = 0;
	j = 0;
	len = ft_word_count(s, c);
	result = malloc(sizeof(char *) * (len + 1));
	if (!result)
		return (NULL);
	while (*s && i < len)
	{
		while (*s == c)
			s++;
		j = 0;
		while (s[j] != c && s[j])
			j++;
		result[i++] = ft_substr(s, 0, j);
		s += j;
	}
	result[i] = NULL;
	return (result);
}

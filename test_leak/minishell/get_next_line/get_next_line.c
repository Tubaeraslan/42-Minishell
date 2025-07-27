/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <ican@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:13:21 by ican              #+#    #+#             */
/*   Updated: 2024/11/22 11:14:59 by ican             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*reader(int fd, char	*str)
{
	char	*buff;
	int		i;

	i = 1;
	buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (NULL);
	while (i != 0 && ft_strchr(str, '\n') == 0)
	{
		i = read(fd, buff, BUFFER_SIZE);
		if (i == -1)
		{
			free(buff);
			free(str);
			return (NULL);
		}
		buff[i] = '\0';
		str = ft_strjoin(str, buff);
	}
	free(buff);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*line;
	char		*main;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	line = reader(fd, line);
	if (!line)
		return (NULL);
	main = ft_line(line);
	line = cont(line);
	return (main);
}

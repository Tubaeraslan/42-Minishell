/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:52:47 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/27 13:36:12 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);

void	*ft_calloc(size_t count, size_t size);

size_t	ft_strlen(const char *s);

char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);

char	*ft_strdup(const char *s1);

#endif
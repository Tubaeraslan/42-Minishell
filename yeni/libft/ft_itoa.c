/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 17:04:27 by teraslan          #+#    #+#             */
/*   Updated: 2024/10/20 11:22:44 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_len(int n, int sign)
{
	int	i;

	i = 0;
	if (n == 0)
	{
		return (1);
	}
	while (n != 0)
	{
		i++;
		n = n / 10;
	}
	if (sign < 0)
	{
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	num;
	int		sign;
	int		len;

	num = n;
	sign = 1;
	if (num < 0)
		sign = -1;
	num = sign * num;
	len = ft_len(n, sign);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len > 0)
	{
		str[--len] = (num % 10) + '0';
		num = num / 10;
	}
	if (sign == -1)
		str[0] = '-';
	return (str);
}

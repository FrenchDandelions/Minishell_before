/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstrs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:44:18 by thole             #+#    #+#             */
/*   Updated: 2023/11/15 13:44:20 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_dprintchar(int c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_print_percent_fd(int fd)
{
	write(fd, "%", 1);
	return (1);
}

int	ft_dprintf_strlen(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (6);
	while (s[i])
		i++;
	return (i);
}

void	ft_dputstrnull(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void	ft_dputstrf_fd(char *s, int fd)
{
	int		i;
	char	*str;

	i = 0;
	if (!s)
	{
		str = "(null)";
		ft_dputstrnull(str, fd);
	}
	else
	{
		while (s[i])
		{
			write(fd, &s[i], 1);
			i++;
		}
	}
}

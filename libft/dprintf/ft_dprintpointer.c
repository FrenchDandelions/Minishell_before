/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printpointer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:05:01 by thole             #+#    #+#             */
/*   Updated: 2023/11/15 15:05:03 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_d_ptr_len(unsigned long long p, char *base)
{
	unsigned long long	i;
	unsigned long long	base_len;
	unsigned long long	n;

	n = p;
	base_len = ft_strlen(base);
	i = 2;
	if (!p)
		return (5);
	while (n != 0)
	{
		i++;
		n /= base_len;
	}
	return (i);
}

void	ft_d_putptr(unsigned long long p, char *base, int fd)
{
	unsigned long long	i;
	unsigned long long	base_len;

	base_len = ft_strlen(base);
	i = p;
	if (i < base_len)
		ft_putchar_fd(base[i], fd);
	else
	{
		ft_putptr(i / base_len, base, fd);
		ft_putptr(i % base_len, base, fd);
	}
}

int	ft_dprintf_pointer(unsigned long long p, char *base, int fd)
{
	if (!p)
		write(fd, "(nil)", 5);
	else
	{
		write(fd, "0x", 2);
		ft_d_putptr(p, base, fd);
	}
	return (ft_d_ptr_len(p, base));
}

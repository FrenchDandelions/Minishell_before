/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbrs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:09:06 by thole             #+#    #+#             */
/*   Updated: 2023/11/15 14:09:08 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_d_nbrlen(int n)
{
	int	i;

	i = 0;
	if (n <= 0)
	{
		i++;
		n *= -1;
	}
	while (n != 0)
	{
		i++;
		n /= 10;
	}
	return (i);
}

int	ft_d_unsigned_len(unsigned int n)
{
	unsigned int	i;

	i = 0;
	if (i == n)
		return (1);
	while (n)
	{
		i++;
		n /= 10;
	}
	return (i);
}

void	ft_dprint_putnbr(int n, int fd)
{
	long int	i;

	i = n;
	if (i < 0)
	{
		write(1, "-", 1);
		i *= -1;
	}
	if (i <= 9)
		ft_putchar_fd(i + '0', fd);
	else
	{
		ft_dprint_putnbr(i / 10, fd);
		ft_dprint_putnbr(i % 10, fd);
	}
}

int	ft_d_unsigned_putnbr(unsigned int n, int fd)
{
	unsigned long int	i;

	i = n;
	if (i <= 9)
		ft_putchar_fd(i + '0', fd);
	else
	{
		ft_d_unsigned_putnbr(i / 10, fd);
		ft_d_unsigned_putnbr(i % 10, fd);
	}
	return (ft_d_unsigned_len(n));
}

int	ft_dprintf_putnbr(int nb, int fd)
{
	ft_dprint_putnbr(nb, fd);
	return (ft_d_nbrlen(nb));
}

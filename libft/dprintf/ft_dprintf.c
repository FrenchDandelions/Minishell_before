/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 12:51:16 by thole             #+#    #+#             */
/*   Updated: 2023/11/15 12:51:19 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_dprintswitch(va_list args, const char c, int fd)
{
	int	len;

	len = 0;
	if (c == 'c')
		len += ft_dprintchar(va_arg(args, int), fd);
	else if (c == 's')
		len += ft_dprintf_string(va_arg(args, char *), fd);
	else if (c == 'p')
		len += ft_dprintf_pointer(va_arg(args, unsigned long long),
				"0123456789abcdef", fd);
	else if (c == 'i' || c == 'd')
		len += ft_dprintf_putnbr(va_arg(args, int), fd);
	else if (c == 'u')
		len += ft_d_unsigned_putnbr(va_arg(args, unsigned int), fd);
	else if (c == 'x')
		len += ft_d_putnbr_hexa_base(va_arg(args, unsigned int),
				"0123456789abcdef", fd);
	else if (c == 'X')
		len += ft_d_putnbr_hexa_base(va_arg(args, unsigned int),
				"0123456789ABCDEF", fd);
	else if (c == '%')
		len += ft_print_percent_fd(fd);
	return (len);
}

int	ft_dprintf(int fd, const char *str, ...)
{
	int		length;
	va_list	args;
	int		i;

	i = 0;
	length = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			length += ft_dprintswitch(args, str[i + 1], fd);
			i += 2;
		}
		else
		{
			write(fd, &str[i], 1);
			length++;
			i++;
		}
	}
	va_end(args);
	return (length);
}

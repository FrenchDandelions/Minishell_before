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

#include "ft_printf.h"

int	ft_printswitch(va_list args, const char c)
{
	int	len;

	len = 0;
	if (c == 'c')
		len += ft_printchar(va_arg(args, int));
	else if (c == 's')
		len += ft_printf_string(va_arg(args, char *));
	else if (c == 'p')
		len += ft_printf_pointer(va_arg(args, unsigned long long),
				"0123456789abcdef", 1);
	else if (c == 'i' || c == 'd')
		len += ft_printf_putnbr(va_arg(args, int));
	else if (c == 'u')
		len += ft_unsigned_putnbr(va_arg(args, unsigned int), 1);
	else if (c == 'x')
		len += ft_putnbr_hexa_base(va_arg(args, unsigned int),
				"0123456789abcdef", 1);
	else if (c == 'X')
		len += ft_putnbr_hexa_base(va_arg(args, unsigned int),
				"0123456789ABCDEF", 1);
	else if (c == '%')
		len += ft_print_percent();
	return (len);
}

int	ft_printf(const char *str, ...)
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
			length += ft_printswitch(args, str[i + 1]);
			i += 2;
		}
		else
		{
			write(1, &str[i], 1);
			length++;
			i++;
		}
	}
	va_end(args);
	return (length);
}

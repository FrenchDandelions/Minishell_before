/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testmain_printf.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 13:26:26 by thole             #+#    #+#             */
/*   Updated: 2024/01/09 13:26:27 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include "../libft.h"

int	main(void)
{
	int				i;
	unsigned int	j;
	char			*str;
	char			*str2;
	char			c;
	void			*p;
	void			*p2;
	int				fd;

	fd = open("test.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	fsync(fd);
	str = NULL;
	str2 = "CHAT";
	c = 'c';
	p = &str2;
	p2 = NULL;
	i = 2147483647;
	j = i * 2 + i / 2;
	dprintf(2, "1\n");
	ft_dprintf(2, "2\n");
	dprintf(fd, "len nor=%d\n", dprintf(fd, "Testd %d\n", i));
	dprintf(fd, "len mine=%d\n", ft_dprintf(fd, "Testd %d\n", i));
	dprintf(fd, "len nor=%d\n", dprintf(fd, "Testc %c\n", c));
	dprintf(fd, "len mine=%d\n", ft_dprintf(fd, "Testc %c\n", c));
	dprintf(fd, "len nor=%d\n", dprintf(fd, "Tests %s\n", str));
	dprintf(fd, "len mine=%d\n", ft_dprintf(fd, "Tests %s\n", str));
	dprintf(fd, "len nor=%d\n", dprintf(fd, "Tests %s\n", str2));
	dprintf(fd, "len mine=%d\n", ft_dprintf(fd, "Tests %s\n", str2));
	dprintf(fd, "len nor=%d\n", dprintf(fd, "Testp %p\n", p));
	dprintf(fd, "len mine=%d\n", ft_dprintf(fd, "Testp %p\n", p));
	dprintf(fd, "len nor=%d\n", dprintf(fd, "Testp %p\n", p2));
	dprintf(fd, "len mine=%d\n", ft_dprintf(fd, "Testp %p\n", p2));
	dprintf(fd, "len nor=%d\n", dprintf(fd, "Testx %x\n", 42));
	dprintf(fd, "len mine=%d\n", ft_dprintf(fd, "Testx %x\n", 42));
	dprintf(fd, "len nor=%d\n", dprintf(fd, "TestX %X\n", 42));
	dprintf(fd, "len mine=%d\n", ft_dprintf(fd, "TestX %X\n", 42));
	dprintf(fd, "len nor=%d\n", dprintf(fd, "Testu %u\n", j));
	dprintf(fd, "len mine=%d\n", ft_dprintf(fd, "Testu %u\n", j));
	close(fd);
}*/

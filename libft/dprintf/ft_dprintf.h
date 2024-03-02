/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 13:22:30 by thole             #+#    #+#             */
/*   Updated: 2024/01/09 13:22:31 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H
# define FT_DPRINTF_H

# include "../libft.h"

int		ft_dprintf(int fd, const char *str, ...);
int		ft_dprintf_string(char *str, int fd);
int		ft_dprintchar(int c, int fd);
int		ft_print_percent_fd(int fd);
int		ft_dprintf_strlen(char *s);
void	ft_dputstrnull(char *s, int fd);
void	ft_dputstrf_fd(char *s, int fd);
int		ft_d_ptr_len(unsigned long long p, char *base);
void	ft_d_putptr(unsigned long long p, char *base, int fd);
int		ft_dprintf_pointer(unsigned long long p, char *base, int fd);
int		ft_d_nbrlen(int n);
int		ft_d_unsigned_len(unsigned int n);
int		ft_dprintf_putnbr(int n, int fd);
int		ft_d_unsigned_putnbr(unsigned int n, int fd);
int		ft_d_hexa_len(unsigned int n, char *base);
int		ft_d_putnbr_hexa_base(unsigned int n, char *base, int fd);
int		ft_dprintf(int fd, const char *str, ...);
void	ft_dprint_putnbr(int n, int fd);
#endif

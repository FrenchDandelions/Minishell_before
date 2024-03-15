/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 14:40:44 by thole             #+#    #+#             */
/*   Updated: 2023/12/10 14:40:46 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t newsize, size_t oldsize)
{
	char	*newptr;
	size_t	cursize;
	size_t	next_size;

	if (ptr == 0)
		return (malloc(newsize));
	cursize = oldsize;
	if (cursize == 0 || cursize == 1)
		cursize = 100;
	if (newsize <= cursize || newsize == 0)
		next_size = cursize * cursize;
	else
		next_size = newsize;
	newptr = malloc(next_size);
	if (!newptr)
		return (free(ptr), NULL);
	ft_bzero(newptr, newsize);
	ft_memcpy(newptr, ptr, oldsize);
	free(ptr);
	return (newptr);
}

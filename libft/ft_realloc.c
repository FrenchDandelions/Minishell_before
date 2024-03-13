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

	if (ptr == 0)
		return (malloc(newsize));
	cursize = oldsize;
	if (newsize <= cursize)
		return (ptr);
	newptr = malloc(newsize);
	if (!newptr)
		return (free(ptr), NULL);
	ft_bzero(newptr, newsize);
	ft_memcpy(newptr, ptr, cursize);
	free(ptr);
	return (newptr);
}

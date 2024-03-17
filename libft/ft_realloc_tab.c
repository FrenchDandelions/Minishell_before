/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 14:13:17 by thole             #+#    #+#             */
/*   Updated: 2024/03/17 14:13:19 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = NULL;
}

char	**ft_realloc_tab(char **ptr, size_t newsize, size_t oldsize)
{
	char	**newptr;
	size_t	i;

	i = 0;
	if (ptr == NULL)
		return (malloc(newsize));
	if (oldsize >= newsize)
		return (ptr);
	newptr = (char **)malloc(sizeof(char *) * (newsize));
	if (!newptr)
		return (free_tab(ptr), NULL);
	while (ptr[i])
	{
		newptr[i] = ft_strdup(ptr[i]);
		if (!newptr[i])
			return (free_tab(newptr), free_tab(ptr), NULL);
		i++;
	}
	while (i < newsize)
	{
		newptr[i] = NULL;
		i++;
	}
	free_tab(ptr);
	return (newptr);
}

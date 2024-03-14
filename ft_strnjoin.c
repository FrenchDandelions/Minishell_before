/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:16:14 by thole             #+#    #+#             */
/*   Updated: 2024/03/13 15:16:15 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

char	*free_string(char *s1, char *s2, int i)
{
	if (i == 0)
		free(s2);
	else
		free(s1);
	return (NULL);
}

char	*ft_n(char *s1, char *s2, size_t size_s1, int start)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (free_string(s1, s2, start));
	i = 0;
	j = 0;
	while (i < size_s1 && s1[start + i])
	{
		str[i] = s1[start + i];
		i++;
	}
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}

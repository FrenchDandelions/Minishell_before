/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   goodbye_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:25:53 by thole             #+#    #+#             */
/*   Updated: 2024/03/08 17:25:54 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	is_quotes(int c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

char	*takeoff_quotes(char *str)
{
	char	*new_str;
	int		i;
	int		j;
	char	type;

	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (free(str), NULL);
	i = 0;
	j = 0;
	type = 'c';
	while (str[i + j])
	{
		if (is_quotes(str[i + j]))
			type = str[i + j];
		else
		{
			new_str[i] = str[i + j];
			i++;
		}
		if (is_quotes(type))
		{
			i++;
			while (str[i + j] != type)
				j++;
			j++;
		}
		i++;
	}
	return (free(str), new_str);
}

int	goodbye_quotes(char **tab)
{
	for (int i = 0; tab[i]; i++)
	{
		if (takeoff_quotes(tab[i]) == NULL)
			return (ERR_MALLOC);
	}
	return (SUCCESS);
}

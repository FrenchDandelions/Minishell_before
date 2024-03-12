/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 18:00:38 by thole             #+#    #+#             */
/*   Updated: 2024/03/12 18:00:39 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	is_alnum_undescore(int c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*expand(char *str, t_struct *s)
{
	char	*string;
	char	*string2;
	int		i;
	int		len;

	i = 0;
	len = 0;
	string = NULL;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] != '\'')
				i++;
			i++;
		}
		if (str[i] == '$')
		{
			i++;
			while (str[i + len] && is_alnum_undescore(str[i + len]))
				len++;
			string = ft_substr(str + i, i, len);
			if (!string)
				return (NULL);
			find_in_path(string)
		}
	}
	return (string2);
}

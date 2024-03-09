/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 15:41:00 by thole             #+#    #+#             */
/*   Updated: 2024/03/09 15:41:01 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	is_alnum_undescore(int c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

int	is_quotes(int c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

char	*fetch_dollar(char *str, char **expanded, int *present, t_struct *s)
{
	int		temp;
	int		size;
	char	*str;

	temp = *present;
	size = 0;
	while (str[temp] && is_alnum_underscore(str[temp]))
	{
		temp++;
		size++;
	}
	if (size == 0 && !is_quotes(str[temp]))
		return ("nice");
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	size = 0;
	while (str[temp] && is_alnum_underscore(str[temp + size]))
	{
		str[size] = str[temp + size];
		size++;
	}
}

char	*expand_dollar(char *str, t_struct *st)
{
	int		present;
	int		past;
	char	*expanded;

	past = 0;
	expanded = calloc(sizeof(char), (ft_strlen(str) + 1));
	if (!expanded)
		return (NULL);
	present = 0;
	while (str[present])
	{
		if (str[present] == '\'')
		{
			expanded[present] = str[present];
			present++;
			while (str[present] != '\'')
			{
				expanded[present] = str[present];
				present++;
			}
			expanded[present] = str[present];
			present++;
		}
		if (str[present] == '$')
		{
			present++;
			past = present;
			if (fetch_dollar(str, &expanded, present, st) == NULL)
				return (NULL);
		}
		expanded[present] = str[present];
		present++;
	}
}

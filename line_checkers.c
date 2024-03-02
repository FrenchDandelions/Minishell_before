/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_checkers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 15:27:12 by thole             #+#    #+#             */
/*   Updated: 2024/02/11 15:27:13 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	ft_is_builtin(char *str)
{
	if (ft_strcmp(str, "cd") == 0)
		return (BUILTIN);
	else if (ft_strcmp(str, "echo") == 0)
		return (BUILTIN);
	else if (ft_strcmp(str, "pwd") == 0)
		return (BUILTIN);
	else if (ft_strcmp(str, "export") == 0)
		return (BUILTIN);
	else if (ft_strcmp(str, "unset") == 0)
		return (BUILTIN);
	else if (ft_strcmp(str, "env") == 0)
		return (BUILTIN);
	else if (ft_strcmp(str, "exit") == 0)
		return (BUILTIN);
	return (0);
}

int	checker_common_parsing_errors(char *line, char to_check)
{
	int	i;
	int	j;
	int	counter;

	i = 0;
	j = 0;
	counter = 0;
	while (line[i])
	{
		while (line[i + j] == to_check)
		{
			counter++;
			j++;
			if (counter > 2)
				return (ERR_PARS);
		}
		i++;
	}
	return (SUCCESS);
}

int	check_line(char *line)
{
	char	*str;
	int		i;
	int		err;

	str = "<>|&";
	i = 0;
	err = quote_checker(line);
	if (err != SUCCESS)
		return (err);
	while (str[i])
	{
		if (checker_common_parsing_errors(line, str[i]) == ERR_PARS)
			return (i - 100);
		i++;
	}
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:20:25 by thole             #+#    #+#             */
/*   Updated: 2024/03/17 17:20:27 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	check_if_is_in_env2(char *env_line, char *var)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (env_line[i] && var[j] && env_line[i] != '=' && env_line[i] == var[j])
	{
		i++;
		j++;
	}
	if (env_line[i] == '=' && !var[j])
		return (1);
	return (0);
}

int	loop_unset(t_struct *s, int j)
{
	int		i;
	char	*temp;

	i = 0;
	while (s->env[i])
	{
		if (check_if_is_in_env2(s->env[i], s->tab[j]))
		{
			free(s->env[i]);
			s->env[i] = NULL;
			while (s->env[i + 1])
			{
				temp = s->env[i];
				s->env[i] = s->env[i + 1];
				s->env[i + 1] = temp;
				i++;
			}
		}
		i++;
	}
	if (s->tab[j + 1])
		return (loop_unset(s, j + 1));
	return (SUCCESS);
}

void	ft_unset(t_struct *s, char **env)
{
	(void)env;
	if (s->tab[1])
		loop_unset(s, 1);
	s->exit_val = 0;
	if (s->count_pipes)
		free_all(s, 0);
	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:07:29 by thole             #+#    #+#             */
/*   Updated: 2024/03/09 17:07:33 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	is_full_of_n(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	else
		return (0);
	while (str[i] == 'n')
		i++;
	if (str[i])
		return (0);
	return (1);
}

int	find_option(char **tab)
{
	int	i;

	i = 1;
	while (tab[i])
	{
		if (ft_strchr(tab[i], '-') && is_full_of_n(tab[i]))
			return (i);
		i++;
	}
	return (0);
}

int	n_case(t_struct *s)
{
	int	i;
	int	j;

	i = 1;
	j = find_option(s->tab);
	if (j)
	{
		while (s->tab[i] && s->tab[i + 1])
		{
			if (i == j)
				i++;
			if (!is_full_of_n(s->tab[i]) && s->tab[i + 1] && (i + 1) != j)
				printf("%s ", s->tab[i]);
			else if ((i + 1) == j)
				printf("%s", s->tab[i]);
			i++;
		}
		if (i != j)
			printf("%s", s->tab[i]);
	}
	return (j);
}

void	ft_echo(t_struct *s)
{
	int	i;

	i = 1;
	if (!n_case(s))
	{
		while (s->tab[i] && s->tab[i + 1])
		{
			printf("%s ", s->tab[i]);
			i++;
		}
		printf("%s\n", s->tab[i]);
	}
	free_tab(s->dup_env);
	exit(0);
}

void	ft_pwd(t_struct *s)
{
	(void)s;
	printf("pwd\n");
	free_tab(s->dup_env);
	exit(0);
}

void	ft_cd(t_struct *s)
{
	(void)s;
	printf("cd\n");
	free_tab(s->dup_env);
	exit(0);
}

void	ft_export(t_struct *s, char **env)
{
	(void)env;
	(void)s;
	printf("export\n");
	free_tab(s->dup_env);
	exit(0);
}

void	ft_unset(t_struct *s, char **env)
{
	(void)env;
	(void)s;
	printf("unset\n");
	free_tab(s->dup_env);
	exit(0);
}

void	ft_env(t_struct *s, char **env, int fake_env)
{
	int	i;

	i = 0;
	(void)env;
	if (fake_env)
	{
		while (s->dup_env[i])
		{
			printf("%s\n", s->dup_env[i]);
			i++;
		}
	}
	else
	{
		while (s->env[i])
		{
			printf("%s\n", s->env[i]);
			i++;
		}
	}
	free_tab(s->dup_env);
	exit(0);
}

void	ft_exit(t_struct *s)
{
	s->exit = EXIT;
	if (s->count_pipes)
	{
		free_tab(s->dup_env);
		exit(0);
	}
}

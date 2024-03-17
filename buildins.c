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
	if (!tab[0][0])
		i++;
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
	if (!s->tab[1])
		printf("\n");
	else if (!n_case(s))
	{
		while (s->tab[i] && s->tab[i + 1])
		{
			printf("%s ", s->tab[i]);
			i++;
		}
		printf("%s\n", s->tab[i]);
	}
	free_all(s, 0);
}

void	ft_pwd(t_struct *s)
{
	char	str[PATH_MAX];

	if (getcwd(str, PATH_MAX))
		printf("%s\n", str);
	else
		perror("getcwd");
	free_all(s, 0);
}

void	ft_cd(t_struct *s)
{
	(void)s;
	printf("cd\n");
	free_all(s, 0);
}

char	*get_color(int i)
{
	int	temp;

	temp = i % 7;
	if (temp == 0)
		return (RED);
	else if (temp == 1)
		return (ORANGE);
	else if (temp == 2)
		return (YELLOW);
	else if (temp == 3)
		return (GREEN);
	else if (temp == 4)
		return (LIGHTB);
	else if (temp == 5)
		return (BLUE);
	return (PURP2);
}

void	sort_and_print_env(char **env, int i, int j)
{
	char	*temp;

	while (env[i])
	{
		j = 0;
		while (env[j + 1])
		{
			if (env[j + 1] && ft_strcmp(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (env[i])
	{
		printf("%s%s\n%s", get_color(i), env[i], RESET);
		i++;
	}
}

void	ft_export(t_struct *s, char **env)
{
	(void)env;
	if (!s->tab[1])
		sort_and_print_env(s->env, 0, 0);
	else
		printf("export\n");
	free_all(s, 0);
}

void	ft_unset(t_struct *s, char **env)
{
	(void)env;
	(void)s;
	printf("unset\n");
	free_all(s, 0);
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
	free_all(s, 0);
}

int	longlonglen(long long int n)
{
	int				i;
	long long int	num;

	i = 0;
	num = n;
	if (num < 0)
		i++;
	while (num)
	{
		i++;
		num /= 10;
	}
	return (i);
}

int	ft_strlen_ps(char *s)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (s[i] == '-')
		i++;
	while (s[i + j])
	{
		if ((s[i + j] == '0' && i == 1) || (s[i + j] == '0' && i == 0))
		{
			while (s[i + j] == '0')
				j++;
			if (!s[i + j])
			{
				i--;
				break ;
			}
		}
		if (s[i + j])
			i++;
	}
	if (s[0] == '+')
		i--;
	return (i);
}

int	is_only_num(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	get_err_string(t_struct *s, int len)
{
	char	*str;
	int		i;

	str = calloc(sizeof(char), len);
	if (!str)
		free_all(s, -2);
	i = 0;
	while (i < len - 1)
	{
		str[i] = s->tab[1][i];
		i++;
	}
	str[i] = '\0';
	s->string_error = str;
}

void	set_error(t_struct *s)
{
	int	len;

	len = (int)(ft_strlen(s->tab[1]) + 1);
	s->exit_arg = 2;
	s->num_err_exit = 1;
	get_err_string(s, len);
}

void	ft_exit(t_struct *s)
{
	long long int	stat;

	s->exit = EXIT;
	if (s->count_pipes)
		free_all(s, 0);
	if (s->tab[1])
	{
		stat = ft_atoll(s->tab[1]);
		if ((longlonglen(stat) != ft_strlen_ps(s->tab[1]))
			|| !is_only_num(s->tab[1]))
			set_error(s);
		else
		{
			if (s->tab[2])
			{
				s->bad_exit = 1;
				s->exit_val = 1;
			}
			else if (stat < 0 || stat > 256)
				s->exit_arg = stat % 256;
			else
				s->exit_arg = stat;
		}
	}
	else
		s->exit_arg = 0;
}

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
	{
		perror("getcwd");
		free_all(s, 1);
	}
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

int	sort_and_print_env(char **env, int i, int j)
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
	return (SUCCESS);
}

int		export_check_value(t_struct *s, int i);

int	printf_and_exit_export(t_struct *s, int i)
{
	ft_dprintf(2, "%sMinishell: export: '%s': not a valid identifier\n", RED,
		s->tab[i], RESET);
	s->exit_val = 1;
	if (s->tab[i + 1])
		return (export_check_value(s, i + 1));
	return (SUCCESS);
}

int	export_value(t_struct *s, int i, int j)
{
	char	*str;

	str = ft_strdup(s->tab[i]);
	if (!str)
	{
		ft_dprintf(2, "Malloc env export\n");
		free_all(s, -2);
	}
	free(s->env[j]);
	s->env[j] = str;
	s->exit_val = 0;
	return (SUCCESS);
}

int	new_value(t_struct *s, int i, int j)
{
	if (j == s->size_env)
	{
		s->env = ft_realloc_tab(s->env, sizeof(char *) * (s->size_env + 2),
				sizeof(char *) * s->size_env);
		if (!s->env)
		{
			ft_dprintf(2, "Malloc env export\n");
			free_all(s, -2);
		}
		s->size_env += 2;
	}
	s->env[j] = ft_strdup(s->tab[i]);
	if (!s->env[j])
	{
		ft_dprintf(2, "Malloc env\n");
		free_all(s, -2);
	}
	if (s->tab[i + 1])
		return (export_check_value(s, i + 1));
	return (SUCCESS);
}

int	check_if_is_in_env(char *env_line, char *var)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (env_line[i] && var[j] && env_line[i] != '=' && var[j] != '='
		&& env_line[i] == var[j])
	{
		i++;
		j++;
	}
	if (env_line[i] == '=' && var[j] == '=')
		return (1);
	return (0);
}

int	loop_export(t_struct *s, int i, int *j)
{
	while (s->env[(*j)])
	{
		if (check_if_is_in_env(s->env[(*j)], s->tab[i]))
		{
			if (export_value(s, i, (*j)) == ERR_MALLOC)
				free_all(s, -2);
			if (s->tab[i + 1])
				return (export_check_value(s, i + 1));
			return (SUCCESS);
		}
		(*j)++;
	}
	return (SUCCESS);
}

int	export_check_value(t_struct *s, int i)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (s->tab[i][k] && is_alnum_undescore(s->tab[i][k])
		&& !ft_isdigit(s->tab[i][0]))
		k++;
	if (k == 0 || (s->tab[i][k] && s->tab[i][k] != '='))
		return (printf_and_exit_export(s, i));
	if (!s->tab[i][k] && !s->tab[i + 1])
		return (SUCCESS);
	else if (!s->tab[i][k] && s->tab[i + 1])
		return (export_check_value(s, i + 1));
	if (loop_export(s, i, &j) == ERR_MALLOC)
		return (ERR_MALLOC);
	if (!s->env[j])
		return (new_value(s, i, j));
	return (SUCCESS);
}

int	ft_export_notchild(t_struct *s, char **env)
{
	(void)env;
	s->exported = 1;
	if (!s->tab[1])
		return (sort_and_print_env(s->env, 0, 0));
	return (export_check_value(s, 1));
}

void	ft_export(t_struct *s, char **env)
{
	(void)env;
	if (!s->tab[1])
		sort_and_print_env(s->env, 0, 0);
	else
		export_check_value(s, 1);
	free_all(s, 0);
}

void	ft_env(t_struct *s, char **env, int fake_env)
{
	int	i;

	i = 0;
	(void)env;
	(void)fake_env;
	while (s->env[i])
	{
		printf("%s\n", s->env[i]);
		i++;
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

	str = ft_calloc(sizeof(char), len);
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

void	set_val(t_struct *s, int i)
{
	if (s->exit_val != 130 && s->exit_val != 131)
	{
		s->bad_exit = i;
		s->exit_val = i;
	}
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
				set_val(s, 1);
			else if (stat < 0 || stat > 256)
				s->exit_arg = stat % 256;
			else
				s->exit_arg = stat;
		}
	}
	// else
	// 	set_val(s, 0);
}

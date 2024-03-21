/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:30:17 by thole             #+#    #+#             */
/*   Updated: 2024/03/19 12:30:18 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

char	*find_home_in_env(char **env, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		if (check_if_is_in_env2(env[i], to_find))
		{
			while (env[i][j] != '=')
				j++;
			return (ft_substr(env[i], j + 1, (int)ft_strlen(env[i]) - j));
		}
		i++;
	}
	return (ft_strdup(""));
}

void	perror_cd(char *s)
{
	ft_dprintf(STDERR_FILENO, "Minishell: cd: ");
	perror(s);
}

int	free_error_cd(t_struct *s, char *home)
{
	if (home)
	{
		perror_cd(home);
		free(home);
	}
	else
		perror_cd(s->tab[1]);
	if (s->count_pipes)
		free_all(s, 1);
	s->exit_val = 1;
	return (1);
}

int	free_cd(t_struct *s, char *home)
{
	if (home)
		free(home);
	if (s->count_pipes)
		free_all(s, 0);
	if (!s->error_cd)
		s->exit_val = 0;
	s->error_cd = 0;
	return (0);
}

int		change_paths(t_struct *s, char *oldpwd, int check);

int	err_home(t_struct *s, char *home)
{
	return (ft_dprintf(STDERR_FILENO, "Minishell: cd: HOME not set\n"),
		free(home), free_all(s, 1), 1);
}

int	go_home(t_struct *s)
{
	char	*home;
	char	pwd[PATH_MAX];
	int		check;

	check = 0;
	home = find_home_in_env(s->env, "HOME");
	if (!home)
	{
		if (s->count_pipes)
			return (ft_dprintf(STDERR_FILENO, "Malloc\n"), free_all(s, -2), -2);
		return (ft_dprintf(STDERR_FILENO, "Malloc\n"), -2);
	}
	if (!home[0])
	{
		if (s->count_pipes)
			return (err_home(s, home));
		return (ft_dprintf(STDERR_FILENO, "Minishell: cd: HOME not set\n"), 1);
	}
	if (!getcwd(pwd, PATH_MAX))
		check = 1;
	if (chdir(home))
		return (free_error_cd(s, home));
	if (change_paths(s, pwd, check) == ERR_MALLOC)
		return (ft_dprintf(STDERR_FILENO, "Malloc\n"), free_cd(s, home), -2);
	return (free_cd(s, home));
}

int	print_error_cd(t_struct *s)
{
	ft_dprintf(STDERR_FILENO, "Minishell: cd: too many arguments\n");
	if (s->count_pipes)
		free_all(s, 1);
	s->exit_val = 1;
	return (1);
}

int	find_path(t_struct *s)
{
	char	pwd[PATH_MAX];
	int		check;

	check = 0;
	if (!getcwd(pwd, PATH_MAX))
		check = 1;
	if (chdir(s->tab[1]))
		return (free_error_cd(s, NULL));
	if (change_paths(s, pwd, check) == ERR_MALLOC)
		return (ft_dprintf(STDERR_FILENO, "Malloc\n"), free_cd(s, NULL), -2);
	return (free_cd(s, NULL));
}

int	change_old(t_struct *s, char *oldpwd, int check)
{
	int		i;
	char	*str;

	i = 0;
	while (s->env[i])
	{
		if (check_if_is_in_env2(s->env[i], "OLDPWD"))
		{
			if (check)
				return (SUCCESS);
			str = ft_strjoin("OLDPWD=", oldpwd);
			if (!str)
				return (ERR_MALLOC);
			free(s->env[i]);
			s->env[i] = str;
		}
		i++;
	}
	return (SUCCESS);
}

int	change_new(t_struct *s, char *pwd, int check)
{
	int		i;
	char	*str;

	i = 0;
	while (s->env[i])
	{
		if (check_if_is_in_env2(s->env[i], "PWD"))
		{
			if (check)
				return (SUCCESS);
			str = ft_strjoin("PWD=", pwd);
			if (!str)
				return (ERR_MALLOC);
			free(s->env[i]);
			s->env[i] = str;
		}
		i++;
	}
	return (SUCCESS);
}

int	change_paths(t_struct *s, char *oldpwd, int check)
{
	char	pwd[PATH_MAX];

	if (change_old(s, oldpwd, check) == ERR_MALLOC)
		return (ERR_MALLOC);
	check = 0;
	if (!getcwd(pwd, PATH_MAX))
	{
		perror("cd: error retrieving current directory: getcwd: cannot access parent directories: ");
		s->exit_val = 1;
		s->error_cd = 1;
		check = 1;
	}
	else if (change_new(s, pwd, check) == ERR_MALLOC)
		return (ERR_MALLOC);
	return (SUCCESS);
}

void	ft_cd(t_struct *s)
{
	int	stat;

	if (!s->tab[1])
		stat = go_home(s);
	else if (s->tab[2])
		stat = print_error_cd(s);
	else
		stat = find_path(s);
	if (stat == ERR_MALLOC)
		ft_dprintf(STDERR_FILENO, "Fatal, malloc error in cd\n");
	if (s->count_pipes)
		free_all(s, 0);
}

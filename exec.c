/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 13:15:13 by thole             #+#    #+#             */
/*   Updated: 2024/03/09 13:15:15 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
	return ;
}

void	exit_error(char *str)
{
	ft_dprintf(2, "%s", str);
	exit(1);
}

void	exit_error_array(char *str, char **array)
{
	ft_dprintf(2, "%s", str);
	free_array(array);
	exit(1);
}

int	err_fork(int *pipe, int check)
{
	if (check == 0)
	{
		close(pipe[0]);
		close(pipe[1]);
		ft_dprintf(2, "fork\n");
		return (ERR_FORK);
	}
	else if (check == 1)
	{
		ft_dprintf(2, "fork\n");
		return (ERR_FORK);
	}
	return (SUCCESS);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**dup_array(char **tab)
{
	char	**dup;
	int		i;

	i = 0;
	dup = NULL;
	while (tab && tab[i])
		i++;
	dup = malloc(sizeof(char *) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (tab && tab[i])
	{
		dup[i] = ft_strdup(tab[i]);
		if (!dup[i])
			return (free_tab(dup), NULL);
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

int	ft_is_buildin(char *s)
{
	if (ft_strncmp("echo", s, 5) == 0)
		return (1);
	else if (ft_strncmp("export", s, 7) == 0)
		return (1);
	else if (ft_strncmp("cd", s, 3) == 0)
		return (1);
	else if (ft_strncmp("pwd", s, 4) == 0)
		return (1);
	else if (ft_strncmp("unset", s, 6) == 0)
		return (1);
	else if (ft_strncmp("env", s, 4) == 0)
		return (1);
	else if (ft_strncmp("exit", s, 5) == 0)
		return (1);
	return (0);
}

void	exec_buildin(t_struct *s, char **env, int i)
{
	if (ft_strncmp("echo", s->tab[0], 5) == 0)
		ft_echo(s);
	else if (ft_strncmp("export", s->tab[0], 7) == 0)
		ft_export(s, env);
	else if (ft_strncmp("cd", s->tab[0], 3) == 0)
		ft_cd(s);
	else if (ft_strncmp("pwd", s->tab[0], 4) == 0)
		ft_pwd(s);
	else if (ft_strncmp("unset", s->tab[0], 6) == 0)
		ft_unset(s, env);
	else if (ft_strncmp("env", s->tab[0], 4) == 0)
		ft_env(s, env, i);
	else if (ft_strncmp("exit", s->tab[0], 5) == 0)
		ft_exit(s);
}

void	exec_normal(t_struct *s, char **env)
{
	char	*path;
	int		i;

	i = 0;
	if (!s->tab[0])
	{
		ft_dprintf(2, "%s", s->tab[0]);
		exit_error_array(": command not found\n", s->dup_env);
	}
	path = get_path(s->tab[0], env, &i);
	if (path == NULL)
		exit_error_array("malloc\n", s->dup_env);
	if (execve(path, s->tab, env) == -1)
	{
		if (errno == EACCES)
		{
			perror(s->tab[0]);
			free_tab(s->dup_env);
			exit(126);
		}
		ft_dprintf(2, "%s: command not found\n", s->tab[0]);
		free_tab(s->dup_env);
		exit(1);
	}
}

int	exec_path(t_struct *s, int index, int fake_env)
{
	if (fake_env)
	{
		if (index == 1)
			exec_buildin(s, s->dup_env, fake_env);
		else
			exec_normal(s, s->dup_env);
	}
	else
	{
		if (index == 1)
			exec_buildin(s, s->env, fake_env);
		else
			exec_normal(s, s->env);
	}
	return (SUCCESS);
}

int	exec(t_struct *s, t_file *file)
{
	if (s->counter != s->count_pipes)
		if (pipe(s->pipe) == -1)
			return (ft_dprintf(2, "pipe\n"), ERR_PIPE);
	s->pid = fork();
	if (s->pid < 0)
		return (err_fork(s->pipe, 0));
	else if (!s->pid)
	{
		s->dup_env = dup_array(s->env);
		if (!s->dup_env)
			return (ERR_MALLOC);
		if (do_files(file, s) == ERR_PARS)
		{
			free_tab(s->dup_env);
			exit(0);
		}
		if (!s->tab[0])
		{
			free_tab(s->dup_env);
			exit(0);
		}
		exec_path(s, ft_is_buildin(s->tab[0]), 1);
	}
	else
	{
		if (!s->is_first)
			close(s->last_fd);
		s->last_fd = s->pipe[0];
		if (s->counter != s->count_pipes)
			close(s->pipe[1]);
		return (SUCCESS);
	}
	return (SUCCESS);
}

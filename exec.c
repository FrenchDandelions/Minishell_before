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

extern int	g_sig;

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

void	exit_error_array(char *str, t_struct *s, char *s1, int index)
{
	if (index == 1 && !s1)
		ft_putstr_fd(s->tab[0], STDERR_FILENO);
	else if (index == 1 && s1 && !s1[0])
		ft_putstr_fd(s1, STDERR_FILENO);
	// if (s1)
	// 	free(s1);
	ft_putendl_fd(str, STDERR_FILENO);
	if (index == 1)
		free_all(s, 127);
	free_all(s, -2);
}

int	err_fork(int *pipe, int check)
{
	if (check == 0)
	{
		close(pipe[0]);
		close(pipe[1]);
		ft_dprintf(2, "fork\n");
		return (-1);
	}
	else if (check == 1)
	{
		ft_dprintf(2, "fork\n");
		return (-1);
	}
	return (SUCCESS);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
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

void	exec_buildin(t_struct *s, char **env, int i, char *cmd)
{
	if (ft_strncmp("echo", cmd, 5) == 0)
		ft_echo(s);
	else if (ft_strncmp("export", cmd, 7) == 0)
		ft_export(s, env);
	else if (ft_strncmp("cd", cmd, 3) == 0)
		ft_cd(s);
	else if (ft_strncmp("pwd", cmd, 4) == 0)
		ft_pwd(s);
	else if (ft_strncmp("unset", cmd, 6) == 0)
		ft_unset(s, env);
	else if (ft_strncmp("env", cmd, 4) == 0)
		ft_env(s, env, i);
	else if (ft_strncmp("exit", cmd, 5) == 0)
		ft_exit(s);
}

void	handle_errno(t_struct *s)
{
	int	err;

	err = errno;
	if (err == EACCES)
	{
		perror(s->tab[0]);
		errno = err;
		free_all(s, 126);
	}
	if (err == ENOENT)
	{
		perror(s->tab[0]);
		errno = err;
		free_all(s, 127);
	}
}

void	exec_normal(t_struct *s, char **env)
{
	char	*path;
	int		i;

	i = 0;
	if (!s->tab[0] || !s->tab[0][0])
		exit_error_array(": command not found\n", s, NULL, 1);
	path = get_path(s->tab[0], env, &i);
	if (path == NULL)
		exit_error_array("malloc\n", s, path, 0);
	if (i == 2)
		exit_error_array("", s, path, 1);
	if (!path[0])
		exit_error_array(": command not found\n", s, path, 1);
	if (execve(path, s->tab, env) == -1)
	{
		handle_errno(s);
		ft_dprintf(2, "%s: command not found\n", s->tab[0]);
		free_all(s, 127);
	}
	if (g_sig == 130)
		free_all(s, 130);
	else if (g_sig == 131)
		free_all(s, 131);
	free_all(s, 0);
}

int	exec_path(t_struct *s, int index, int fake_env)
{
	char	*cmd;

	if (s->tab[0][0])
		cmd = s->tab[0];
	else
		cmd = s->tab[1];
	if (fake_env)
	{
		if (index == 1)
			exec_buildin(s, s->dup_env, fake_env, cmd);
		else
			exec_normal(s, s->dup_env);
	}
	else
	{
		if (index == 1)
			exec_buildin(s, s->env, fake_env, cmd);
		else
			exec_normal(s, s->env);
	}
	return (SUCCESS);
}

int	exec(t_struct *s, t_file *file)
{
	if (s->counter != s->count_pipes)
		if (pipe(s->pipe) == -1)
			return (ft_dprintf(2, "pipe\n"), -1);
	sig_child();
	s->pid = fork();
	if (s->pid < 0)
		return (err_fork(s->pipe, 0));
	else if (!s->pid)
	{
		if (do_files(file, s))
			free_all(s, 1);
		if (!s->tab[0])
		{
			if (s->count_pipes && s->here_doc_open)
				close(s->here_doc[0]);
			free_all(s, 0);
		}
		if (s->tab[0])
			exec_path(s, ft_is_buildin(s->tab[0]), 1);
	}
	else
	{
		if (!s->is_first)
			close(s->last_fd);
		s->last_fd = s->pipe[0];
		if (s->counter != s->count_pipes)
			close(s->pipe[1]);
		g_sig = 0;
		return (SUCCESS);
	}
	return (SUCCESS);
}

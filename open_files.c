/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 14:48:27 by thole             #+#    #+#             */
/*   Updated: 2024/03/09 14:48:28 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	open_output(char *s, int token, t_struct *st)
{
	char	*str;
	int		fd;

	str = ft_strdup(s);
	// (void)st;
	// str = expand_dollar(str, st);
	if (!str)
		return (ERR_MALLOC);
	if (token != TK_ARG)
	{
		str = takeoff_quotes(str);
		if (!str)
			return (ERR_MALLOC);
	}
	fd = open(str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(str);
		free(str);
		return (ERR_PARS);
	}
	close(fd);
	if (st->outfile)
		free(st->outfile);
	st->outfile = ft_strdup(str);
	free(str);
	return (0);
}

int	open_append(char *s, int token, t_struct *st)
{
	char	*str;
	int		fd;

	str = ft_strdup(s);
	// (void)st;
	// str = expand_dollar(str, st);
	if (!str)
		return (ERR_MALLOC);
	if (token != TK_ARG)
	{
		str = takeoff_quotes(str);
		if (!str)
			return (ERR_MALLOC);
	}
	fd = open(str, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(str);
		free(str);
		return (ERR_PARS);
	}
	close(fd);
	if (st->outfile)
		free(st->outfile);
	st->outfile = ft_strdup(str);
	free(str);
	return (0);
}

int	open_input(char *s, int token, t_struct *st)
{
	char	*str;
	int		fd;

	str = ft_strdup(s);
	// (void)st;
	// str = expand_dollar(str, st);
	if (!str)
		return (ERR_MALLOC);
	if (token != TK_ARG)
	{
		str = takeoff_quotes(str);
		if (!str)
			return (ERR_MALLOC);
	}
	fd = open(str, O_RDONLY, 0644);
	if (fd == -1)
	{
		perror(str);
		free(str);
		return (-1);
	}
	close(fd);
	if (st->infile)
		free(st->infile);
	st->infile = ft_strdup(str);
	free(str);
	return (0);
}

char	*ft_strjoin_nl(char *s1, char *s2)
{
	char	*str;
	char	*s;

	str = ft_strjoin(s1, s2);
	if (!str)
		return (free(s1), free(s2), NULL);
	s = ft_strjoin(str, "\n");
	free(str);
	if (!s)
		return (free(s1), free(s2), NULL);
	return (free(s1), free(s2), s);
}

int	join_heredoc(char **string, char *str, int i)
{
	char	*temp;

	if (i == 0)
	{
		temp = ft_strjoin_nl((*string), str);
		if (!temp)
			return (ERR_MALLOC);
		(*string) = temp;
	}
	else
	{
		temp = ft_strjoin((*string), str);
		if (!temp)
			return (ERR_MALLOC);
		(*string) = temp;
	}
	return (0);
}

int	split_and_expand(char *s, int fd, t_struct *st)
{
	char	**tab;
	char	*str;
	char	*string;
	int		i;

	i = 0;
	str = NULL;
	(void)st;
	tab = ft_split(s, '\n');
	if (!tab)
		return (ERR_MALLOC);
	while (tab[i])
	{
		// str = expand_dollar(tab[i], st);
		// if (!str)
		// 	return (free(tab), ERR_MALLOC);
		if (tab[i + 1])
		{
			if (join_heredoc(&string, str, 0) == ERR_MALLOC)
				return (free_tab(tab), ERR_MALLOC);
		}
		if (join_heredoc(&string, str, 1) == ERR_MALLOC)
			return (free_tab(tab), ERR_MALLOC);
		i++;
	}
	return (ft_dprintf(fd, "%s", string), 0);
}

int	open_dlmtr(char *s, int token, t_struct *st)
{
	int	status;

	if (!st->here_doc_open)
	{
		if (pipe(st->here_doc) == -1)
			return (ERR_PIPE);
	}
	else
	{
		close(st->here_doc[0]);
		if (pipe(st->here_doc) == -1)
			return (ERR_PIPE);
	}
	if (token != TK_ARG)
	{
		status = split_and_expand(s, st->here_doc[1], st);
		st->here_doc_open++;
		if (status)
			return (close(st->here_doc[1]), status);
		return (close(st->here_doc[1]), 0);
	}
	ft_dprintf(st->here_doc[1], "%s", s);
	close(st->here_doc[1]);
	st->here_doc_open++;
	return (0);
}

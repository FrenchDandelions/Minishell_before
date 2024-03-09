/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 14:26:46 by thole             #+#    #+#             */
/*   Updated: 2024/03/09 14:26:48 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	get_files(t_file *f, t_struct *s)
{
	int	i;

	i = 0;
	while (f->files[i])
	{
		if (token_redirection(f->modes[i], 1) == 2)
		{
			// s->infile = f->files[i];
			s->token_in = f->token[i];
			s->mode_in = f->modes[i];
		}
		else if (token_redirection(f->modes[i], 1) == 1)
		{
			// s->outfile = f->files[i];
			s->token_out = f->token[i];
			s->mode_out = f->modes[i];
		}
		i++;
	}
	dprintf(2, "%s\n", s->outfile);
}

int	open_out(t_file *f, t_struct *s, int i)
{
	int	status;

	if (f->modes[i] == TK_APPEND)
	{
		status = open_append(f->files[i], f->token[i], s);
		if (status)
			return (status);
	}
	else if (f->modes[i] == TK_OUTPUT)
	{
		status = open_output(f->files[i], f->token[i], s);
		if (status)
			return (status);
	}
	return (0);
}

int	open_in(t_file *f, t_struct *s, int i)
{
	int	status;

	if (f->modes[i] == TK_INPUT)
	{
		status = open_input(f->files[i], f->token[i], s);
		if (status)
			return (status);
	}
	else if (f->modes[i] == TK_DLMTR)
	{
		status = open_dlmtr(f->files[i], f->token[i], s);
		if (status)
			return (status);
	}
	return (0);
}

int	open_files(t_file *f, t_struct *s)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (f->files[i])
	{
		dprintf(2, "Coucou ici : %s\n", f->files[i]);
		if (f->modes[i] == TK_APPEND || f->modes[i] == TK_OUTPUT)
		{
			status = open_out(f, s, i);
			if (status)
				return (status);
		}
		else if (f->modes[i] == TK_INPUT || f->modes[i] == TK_DLMTR)
		{
			status = open_in(f, s, i);
			if (status)
				return (status);
		}
		i++;
	}
	dprintf(2, "Coucou encore : %s\n", s->outfile);
	return (SUCCESS);
}

int	open_struct_file(char *s, int mode, t_struct *st)
{
	int	fd;

	if (mode == TK_APPEND)
	{
		fd = open(s, O_CREAT | O_APPEND | O_WRONLY, 0644);
		return (fd);
	}
	else if (mode == TK_OUTPUT)
	{
		fd = open(s, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		return (fd);
	}
	else if (mode == TK_INPUT)
	{
		fd = open(s, O_RDONLY, 0644);
		return (fd);
	}
	else if (mode == TK_DLMTR)
	{
		fd = st->here_doc[0];
		return (fd);
	}
	return (SUCCESS);
}

int	do_files(t_file *f, t_struct *s)
{
	get_files(f, s);
	s->here_doc_open = 0;
	if (open_files(f, s) == ERR_PARS)
		return (ERR_PARS);
	if (s->infile)
	{
		s->fd_in = open_struct_file(s->infile, s->mode_in, s);
		dup2(s->fd_in, STDIN_FILENO);
		close(s->fd_in);
		free(s->infile);
	}
	if (s->outfile)
	{
		s->fd_out = open_struct_file(s->outfile, s->mode_out, s);
		dup2(s->fd_out, STDOUT_FILENO);
		close(s->fd_out);
		free(s->outfile);
	}
	if (!s->end)
	{
		close(s->pipe[0]);
		dup2(s->pipe[1], STDOUT_FILENO);
		close(s->pipe[1]);
	}
	else
	{
		close(s->pipe[0]);
		dup2(s->pipe[1], STDOUT_FILENO);
		close(s->pipe[1]);
	}
	return (SUCCESS);
}

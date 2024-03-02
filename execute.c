/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 14:43:19 by thole             #+#    #+#             */
/*   Updated: 2024/03/02 14:43:20 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	flush_array(char **tab)
{
	int	i;
	int	k;

	k = 0;
	i = 0;
	while (tab[i] != NULL)
	{
		free(tab[i]);
		i++;
	}
}

char	*epur_str(char *s)
{
	char	*str;

	str = ft_strdup(s);
	if (!str)
		return (NULL);
	return (str);
}

void	epur_commands(t_struct **s)
{
	(void)s;
	return ;
}

int	token_redirection(int token, int mode)
{
	if (mode == 0)
	{
		if (token == TK_APPEND || token == TK_DLMTR || token == TK_OUTPUT
			|| token == TK_INPUT)
			return (1);
	}
	else if (mode == 1)
	{
		if (token == TK_APPEND || token == TK_OUTPUT)
			return (1);
		else if (token == TK_DLMTR || token == TK_INPUT)
			return (2);
	}
	else if (mode == 3)
	{
		if (token == TK_APPEND || token == TK_DLMTR || token == TK_OUTPUT
			|| token == TK_INPUT || token == TK_PIPES || token == TK_PRIO)
			return (1);
	}
	return (0);
}

int	check_if_isatty(char *file, int token)
{
	int	fd;

	if (token == TK_OUTPUT)
	{
		file = epur_str(file);
		if (!file)
			return (ERR_MALLOC);
		fd = open(file, O_TRUNC | O_WRONLY, 0666);
		if (fd == -1)
		{
			free(file);
			return (IS_TTY);
		}
		if (isatty(fd))
			return (close(fd), free(file), IS_TTY);
		return (close(fd), free(file), SUCCESS);
	}
	else if (token == TK_INPUT)
	{
		file = epur_str(file);
		if (!file)
			return (ERR_MALLOC);
		fd = open(file, O_RDONLY, 0666);
		if (fd == -1)
		{
			free(file);
			return (IS_TTY);
		}
		if (isatty(fd))
			return (close(fd), free(file), IS_TTY);
		return (close(fd), free(file), SUCCESS);
	}
	else if (token == TK_APPEND)
	{
		file = epur_str(file);
		if (!file)
			return (ERR_MALLOC);
		fd = open(file, O_APPEND | O_WRONLY, 0666);
		if (fd == -1)
		{
			free(file);
			return (IS_TTY);
		}
		if (isatty(fd))
			return (close(fd), free(file), IS_TTY);
		return (close(fd), free(file), SUCCESS);
	}
	return (SUCCESS);
}

int	create_new_file(t_file *f, char *file, int type)
{
	int	check;

	check = check_if_isatty(file, type);
	if (check == IS_TTY)
		return (SUCCESS);
	else if (check == ERR_MALLOC)
		return (ERR_MALLOC);
	check = token_redirection(type, 1);
	if (check == 1)
	{
		if (f->outfile)
			free(f->outfile);
		f->mode_out = type;
		f->outfile = ft_strdup(file);
		if (!f->outfile)
			return (ERR_MALLOC);
	}
	else if (check == 2)
	{
		if (f->infile)
			free(f->infile);
		f->mode_in = type;
		f->infile = ft_strdup(file);
		if (!f->infile)
			return (ERR_MALLOC);
	}
	return (SUCCESS);
}

int	is_arg(int token)
{
	if (token == TK_ARG || token == TK_WILD || token == TK_DOUBLE
		|| token == TK_SINGLE)
		return (1);
	return (0);
}

void	set_files(t_file *file)
{
	file->infile = NULL;
	file->outfile = NULL;
	file->mode_in = NOTHING;
	file->mode_out = NOTHING;
}

void	flush_files(t_file *file)
{
	if (file->infile != NULL)
		free(file->infile);
	if (file->outfile != NULL)
		free(file->outfile);
	set_files(file);
}

int	fill_cmd_tab(t_last_list **list, t_struct **s)
{
	int	fullness;

	fullness = 0;
	while ((*list)->next)
	{
		if (token_redirection((*list)->token, 3))
			break ;
		if (fullness + 1 == 4096)
		{
			printf("Error\nToo many commands\n");
			(*s)->tab[fullness] = NULL;
			flush_array((*s)->tab);
			return (ERR_MALLOC);
		}
		(*s)->tab[fullness] = ft_strdup((*list)->str);
		if (!(*s)->tab[fullness])
			return (flush_array((*s)->tab), ERR_MALLOC);
		fullness++;
		(*list) = (*list)->next;
	}
	(*s)->tab[fullness] = NULL;
	return (SUCCESS);
}

int	recursive_priority(t_struct *s, t_last_list **list)
{
	t_last_list	*temp;
	int			prio;

	prio = 1;
	(*list) = (*list)->next;
	temp = (t_last_list *)malloc(sizeof(t_last_list));
	if (!temp)
		return (ft_free_changed_list(temp), ERR_MALLOC);
	temp->prev = NULL;
	temp->next = NULL;
	temp->str = NULL;
	temp->token = NOTHING;
	while (prio != 0 && (*list)->next)
	{
		if ((*list)->token == TK_PRIO)
			prio++;
		else if ((*list)->token == TK_END_PRIO)
			prio--;
		if (prio != 0)
		{
			if (temp->str == NULL && temp->token == NOTHING)
			{
				temp->str = ft_strdup((*list)->str);
				if (!temp->str)
					return (ft_free_changed_list(temp), ERR_MALLOC);
				temp->token = (*list)->token;
			}
			else
			{
				temp->next = (t_last_list *)malloc(sizeof(t_last_list));
				if (!temp)
					return (ft_free_changed_list(temp), ERR_MALLOC);
				temp->next->prev = temp;
				temp = temp->next;
				temp->str = ft_strdup((*list)->str);
				if (!temp->str)
					return (ft_free_changed_list(temp), ERR_MALLOC);
				temp->token = (*list)->token;
			}
		}
		(*list) = (*list)->next;
	}
	if (execute(s, temp) == ERR_MALLOC)
		return (ft_free_changed_list(temp), ERR_MALLOC);
	ft_free_changed_list(temp);
	return (SUCCESS);
}

void	do_exec(t_struct **s, t_file *file)
{
	int	i;

	i = 0;
	if ((*s)->tab[i])
		printf("Command : %s\n", (*s)->tab[i]);
	i++;
	while ((*s)->tab[i])
	{
		printf("Command : %s\n", (*s)->tab[i]);
		i++;
	}
	printf("Infile : %s, Mode infile : %d, Outfile : %s, Mode Outfile : %d\n",
		file->infile, file->mode_in, file->outfile, file->mode_out);
}

int	execute(t_struct *s, t_last_list *list)
{
	t_file	file;

	set_files(&file);
	while (list->next)
	{
		if (list->token == TK_PRIO)
		{
			if (recursive_priority(s, &list) == ERR_MALLOC)
				return (ERR_MALLOC);
		}
		if (token_redirection(list->token, 0))
		{
			while (list->next && list->next->str
				&& token_redirection(list->token, 0))
			{
				if (create_new_file(&file, list->next->str,
						list->token) == ERR_MALLOC)
					return (ERR_MALLOC);
				list = list->next->next;
			}
		}
		else if (is_arg(list->token))
		{
			if (fill_cmd_tab(&list, &s) == ERR_MALLOC)
				return (flush_files(&file), ERR_MALLOC);
			if (token_redirection(list->token, 0))
			{
				while (list->next && list->next->str
					&& token_redirection(list->token, 0))
				{
					if (create_new_file(&file, list->next->str,
							list->token) == ERR_MALLOC)
						return (flush_array(s->tab), ERR_MALLOC);
					list = list->next->next;
				}
			}
			epur_commands(&s);
			do_exec(&s, &file);
			flush_files(&file);
			flush_array(s->tab);
		}
		else if (list->next && list->next->token == TK_AND)
		{
			// if (s->err == FAILURE)
			// 	go_to_next_stop(&list);
			;
		}
		if (list->next)
			list = list->next;
	}
	return (SUCCESS);
}

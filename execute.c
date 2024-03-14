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

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	tab[0] = NULL;
}

char	*epur_str(char *s)
{
	char	*str;

	str = ft_strdup(s);
	if (!str)
		return (NULL);
	return (str);
}

int	epur_files(t_file *f, t_struct **s, int i)
{
	char	*dup;

	if (f->files[i])
	{
		dup = takeoff_quotes(expand(f->files[i], (*s)->env, *s));
		if (!dup)
			return (ERR_MALLOC);
		f->files[i] = dup;
		i++;
	}
	if (f->files[i])
		return (epur_files(f, s, i));
	return (SUCCESS);
}

int	epur_commands(t_struct **s, t_file *f, int i)
{
	char	*dup;

	if ((*s)->tab[i])
	{
		dup = takeoff_quotes(expand((*s)->tab[i], (*s)->env, *s));
		if (!dup)
			return (ERR_MALLOC);
		free((*s)->tab[i]);
		(*s)->tab[i] = dup;
		i++;
	}
	if ((*s)->tab[i])
		return (epur_commands(s, f, i));
	return (epur_files(f, s, 0));
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
			|| token == TK_INPUT || token == TK_PIPES || token == TK_PRIO
			|| token == TK_AND || token == TK_OR)
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
		fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
		if (fd == -1)
		{
			free(file);
			return (SUCCESS);
		}
		printf("Here: %d\n", fd);
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
			return (SUCCESS);
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
		fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0666);
		if (fd == -1)
		{
			free(file);
			return (SUCCESS);
		}
		if (isatty(fd))
			return (close(fd), free(file), IS_TTY);
		return (close(fd), free(file), SUCCESS);
	}
	return (SUCCESS);
}

int	create_new_file(t_file *f, char *file, int type, int token)
{
	int	i;

	i = 0;
	while (f->files[i])
		i++;
	if (i == SIZE)
		return (ERR_MALLOC);
	f->modes[i] = type;
	f->files[i] = file;
	if (!f->files[i])
		return (ERR_MALLOC);
	f->files[i + 1] = NULL;
	f->token[i] = token;
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
	file->files[0] = NULL;
	file->modes[0] = NOTHING;
	file->token[0] = NOTHING;
}

void	flush_files(t_file *file, t_struct *s)
{
	int	i;

	i = 0;
	while (file->files[i] != NULL)
	{
		free(file->files[i]);
		file->modes[i] = NOTHING;
		file->token[i] = NOTHING;
		i++;
	}
	s->infile = NULL;
	s->outfile = NULL;
	s->mode_in = NOTHING;
	s->mode_out = NOTHING;
	s->token_in = NOTHING;
	s->token_out = NOTHING;
	set_files(file);
}

int	fill_cmd_tab(t_last_list **list, t_struct **s)
{
	int	fullness;
	int	i;

	i = 0;
	while ((*s)->tab[i])
		i++;
	fullness = i;
	if ((*list)->token == TK_END)
		return ((*s)->tab[fullness] = NULL, SUCCESS);
	while ((*list)->next)
	{
		if (token_redirection((*list)->token, 3))
			break ;
		if (fullness + 1 == SIZE)
		{
			printf("Error\nToo many commands\n");
			(*s)->tab[fullness] = NULL;
			flush_array((*s)->tab);
			return (ERR_MALLOC);
		}
		if ((*list)->str)
		{
			(*s)->tab[fullness] = ft_strdup((*list)->str);
			if (!(*s)->tab[fullness])
				return (flush_array((*s)->tab), ERR_MALLOC);
		}
		else
		{
			(*s)->tab[fullness] = ft_strdup("here_doc.txt");
			if (!(*s)->tab[fullness])
				return (flush_array((*s)->tab), ERR_MALLOC);
		}
		fullness++;
		(*list) = (*list)->next;
	}
	(*s)->tab[fullness] = NULL;
	return (SUCCESS);
}

static t_last_list	*new_node(void)
{
	t_last_list	*list;

	list = malloc(sizeof(t_last_list));
	if (!list)
		return (NULL);
	list->next = NULL;
	list->prev = NULL;
	list->token = -100;
	return (list);
}

static int	create(t_last_list **list)
{
	if ((*list)->token == -100 || !(*list)->str)
	{
		if ((*list)->str)
			free((*list)->str);
		(*list)->str = NULL;
		return (SUCCESS);
	}
	(*list)->next = new_node();
	if (!(*list)->next)
		return (ERR_MALLOC);
	(*list)->next->prev = (*list);
	(*list) = (*list)->next;
	(*list)->str = NULL;
	return (SUCCESS);
}

int	recursive_priority(t_struct *s, t_last_list **list, int depth, int pipe)
{
	t_last_list	*temp;
	t_last_list	*head;
	int			prio;

	prio = 1;
	(*list) = (*list)->next;
	// head = NULL;
	head = (t_last_list *)malloc(sizeof(t_last_list));
	if (!head)
		return (ft_free_changed_list(head), printf("Malloc\n"), ERR_MALLOC);
	head->prev = NULL;
	head->next = NULL;
	head->str = NULL;
	head->token = NOTHING;
	temp = head;
	while (prio != 0 && (*list)->token != TK_END)
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
					return (ft_free_changed_list(head), ERR_MALLOC);
				temp->token = (*list)->token;
			}
			else
			{
				if (create(&temp) == ERR_MALLOC)
					return (ft_free_changed_list(head), ERR_MALLOC);
				temp->str = ft_strdup((*list)->str);
				if (!temp->str)
					return (ft_free_changed_list(head), ERR_MALLOC);
				temp->token = (*list)->token;
			}
			(*list) = (*list)->next;
		}
	}
	if (temp->token != TK_END)
	{
		temp->next = new_list();
		if (!temp->next)
			return (ft_free_changed_list(head), ERR_MALLOC);
		temp->next->prev = temp;
		temp->next->str = NULL;
		temp = temp->next;
		temp->token = TK_END;
	}
	temp->next = NULL;
	if (temp->str)
	{
		free(temp->str);
		temp->str = NULL;
	}
	temp = head;
	// ft_print_list2(*list);
	// ft_print_list2(head);
	depth++;
	if (execute(s, head, depth, pipe) == ERR_MALLOC)
		return (ft_free_changed_list(head), ERR_MALLOC);
	ft_free_changed_list(head);
	return (SUCCESS);
}

/*here, this one,if it's exit the exit variable should be set to EXIT,
	that's the only thing you need to worry about,
	also stat is kinda useless but it's because i don't get
	how the export and stuff with pipes and && and || work,
	so feel free to use it or not,
	and if stat is at 1,it means that we're inside a && or a
		|| so you shouldn't pipe
*/
int	do_exec(t_struct **s, t_file *file, int stat)
{
	if ((*s)->tab[0] && ft_strcmp((*s)->tab[0], "exit") == 0
		&& (*s)->count_pipes == 0)
		return (ft_exit(*s), EXIT);
	// if ((*s)->tab[0])
	stat = exec(*s, file);
	(*s)->is_first = 0;
	(*s)->counter++;
	if (stat != SUCCESS)
		return (stat);
	return (SUCCESS);
}

int	recursive_tab_filler(t_struct **s, t_last_list **list, t_file *file)
{
	if (fill_cmd_tab(&(*list), &(*s)) == ERR_MALLOC)
		return (flush_files(file, *s), ERR_MALLOC);
	if (token_redirection((*list)->token, 0))
	{
		while ((*list)->next && (*list)->next->str
			&& token_redirection((*list)->token, 0))
		{
			if (create_new_file(file, (*list)->next->str, (*list)->token,
					(*list)->next->token) == ERR_MALLOC)
				return (flush_array((*s)->tab), flush_files(file, *s),
					ERR_MALLOC);
			(*list) = (*list)->next->next;
		}
	}
	if (is_arg((*list)->token))
		return (recursive_tab_filler(&(*s), &(*list), file));
	return (SUCCESS);
}

/*function to fill the file struct and command tab,
	you have to check the s->tab for the commands and the file struct passed as parameters,
	even if there is no command, the files should be opened,
		just fill the do_exec function*/
int	execute(t_struct *s, t_last_list *list, int depth, int pipe)
{
	t_file	file;
	int		status;

	set_files(&file);
	s->end = 0;
	while (list->next)
	{
		if (list->token == TK_PRIO)
		{
			status = recursive_priority(s, &list, depth, pipe);
			if (status != SUCCESS)
				return (status);
		}
		else if (is_arg(list->token) || token_redirection(list->token, 0))
		{
			status = recursive_tab_filler(&s, &list, &file);
			if (status != SUCCESS)
				return (status);
		}
		if (list->token == TK_PIPES || list->token == TK_END)
		{
			epur_commands(&s, &file, 0);
			s->file = &file;
			s->is_pipe = 1;
			if (list->token == TK_END)
			{
				s->end = 1;
				s->is_last = 1;
			}
			status = do_exec(&s, &file, 0);
			if (status != SUCCESS)
				return (flush_files(&file, s), flush_array(s->tab), status);
			flush_files(&file, s);
			flush_array(s->tab);
			if (list->token == TK_END && s->exit == EXIT && depth == 0)
				return (EXIT);
		}
		else if (list->token == TK_AND)
		{
			printf("\n&&\n");
			epur_commands(&s, &file, 0);
			s->is_pipe = 0;
			status = do_exec(&s, &file, 1);
			if (status != SUCCESS)
				return (flush_files(&file, s), flush_array(s->tab), status);
			flush_files(&file, s);
			flush_array(s->tab);
			if (s->err == FAILURE)
				go_to_next_stop(&list);
		}
		else if (list->token == TK_OR)
		{
			printf("\n||\n");
			epur_commands(&s, &file, 0);
			s->is_pipe = 0;
			status = do_exec(&s, &file, 2);
			if (status != SUCCESS)
				return (flush_files(&file, s), flush_array(s->tab), status);
			flush_files(&file, s);
			flush_array(s->tab);
			if (s->err == WINNING)
				go_to_next_stop(&list);
		}
		if (list->next)
			list = list->next;
	}
	return (SUCCESS);
}

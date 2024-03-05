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
	int	check;
	int	i;

	i = 0;
	// check = check_if_isatty(file, type);
	// if (check == IS_TTY)
	// 	return (SUCCESS);
	// else if (check == ERR_MALLOC)
	// 	return (ERR_MALLOC);
	check = token_redirection(type, 1);
	if (check == 1)
	{
		while (f->outfile[i])
			i++;
		if (i == SIZE)
			return (ERR_MALLOC);
		f->mode_out = type;
		f->outfile[i] = ft_strdup(file);
		if (!f->outfile[i])
			return (ERR_MALLOC);
		f->outfile[i + 1] = NULL;
	}
	else if (check == 2)
	{
		while (f->infile[i])
			i++;
		if (i == SIZE)
			return (ERR_MALLOC);
		f->mode_in = type;
		f->infile[i] = ft_strdup(file);
		if (!f->infile[i])
			return (ERR_MALLOC);
		f->infile[i + 1] = NULL;
		if (type == TK_DLMTR)
		{
			if (f->limit_heredoc + 1 == SIZE)
				return (ERR_MALLOC);
			f->token[f->limit_heredoc] = token;
			f->limit_heredoc++;
		}
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
	file->infile[0] = NULL;
	file->outfile[0] = NULL;
	file->mode_in = NOTHING;
	file->mode_out = NOTHING;
	file->limit_heredoc = 0;
}

void	flush_files(t_file *file)
{
	int	i;

	i = 0;
	while (file->infile[i] != NULL)
	{
		free(file->infile[i]);
		i++;
	}
	i = 0;
	while (file->outfile[i] != NULL)
	{
		free(file->outfile[i]);
		i++;
	}
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
	ft_print_list2(*list);
	ft_print_list2(head);
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
	int	i;
	int	j;

	i = 0;
	j = 0;
	(void)stat;
	if ((*s)->tab[i])
	{
		if (strcmp((*s)->tab[i], "exit") == 0)
			return ((*s)->exit = EXIT, EXIT);
	}
	if ((*s)->tab[i])
		printf("Command : %s\n", (*s)->tab[i]);
	i++;
	if ((*s)->tab[i - 1])
	{
		while ((*s)->tab[i])
		{
			printf("Command : %s\n", (*s)->tab[i]);
			i++;
		}
	}
	i = 0;
	while (file->infile[i])
		i++;
	while (file->outfile[j])
		j++;
	if (i > 0)
		i--;
	if (j > 0)
		j--;
	printf("Infile : %s, Mode infile : %d, Outfile : %s, Mode Outfile : %d\n",
		file->infile[i], file->mode_in, file->outfile[j], file->mode_out);
	return (SUCCESS);
}

int	recursive_tab_filler(t_struct **s, t_last_list **list, t_file *file)
{
	if (fill_cmd_tab(&(*list), &(*s)) == ERR_MALLOC)
		return (flush_files(file), ERR_MALLOC);
	if (token_redirection((*list)->token, 0))
	{
		while ((*list)->next && (*list)->next->str
			&& token_redirection((*list)->token, 0))
		{
			if (create_new_file(file, (*list)->next->str, (*list)->token,
					(*list)->next->token) == ERR_MALLOC)
				return (flush_array((*s)->tab), flush_files(file), ERR_MALLOC);
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
	while (list->next)
	{
		if (list->token == TK_PRIO)
		{
			status = recursive_priority(s, &list, depth, pipe);
			if (status == ERR_MALLOC)
				return (ERR_MALLOC);
		}
		else if (token_redirection(list->token, 0))
		{
			while (list->next && list->next->str
				&& token_redirection(list->token, 0))
			{
				status = create_new_file(&file, list->next->str, list->token,
						list->next->token);
				if (status == ERR_MALLOC)
					return (ERR_MALLOC);
				list = list->next->next;
			}
		}
		else if (is_arg(list->token))
		{
			status = recursive_tab_filler(&s, &list, &file);
			if (status == ERR_MALLOC)
				return (ERR_MALLOC);
		}
		if (list->token == TK_PIPES || list->token == TK_END)
		{
			epur_commands(&s);
			status = do_exec(&s, &file, 0);
			if (status == ERR_MALLOC)
				return (flush_files(&file), flush_array(s->tab), ERR_MALLOC);
			flush_files(&file);
			flush_array(s->tab);
			if (list->token == TK_END && s->exit == EXIT && depth == 0)
				return (EXIT);
		}
		else if (list->token == TK_AND)
		{
			printf("\n&&\n");
			epur_commands(&s);
			status = do_exec(&s, &file, 1);
			if (status == ERR_MALLOC)
				return (flush_files(&file), flush_array(s->tab), ERR_MALLOC);
			flush_files(&file);
			flush_array(s->tab);
			if (s->err == FAILURE)
			{
				status = execute(s, list, depth, 0);
				if (status == ERR_MALLOC)
					return (flush_files(&file), flush_array(s->tab),
						ERR_MALLOC);
			}
		}
		else if (list->token == TK_OR)
		{
			printf("\n||\n");
			epur_commands(&s);
			status = do_exec(&s, &file, 1);
			if (status == ERR_MALLOC)
				return (flush_files(&file), flush_array(s->tab), ERR_MALLOC);
			flush_files(&file);
			flush_array(s->tab);
			if (s->err == WINNING)
			{
				status = execute(s, list, depth, 0);
				if (status == ERR_MALLOC)
					return (flush_files(&file), flush_array(s->tab),
						ERR_MALLOC);
			}
		}
		if (list->next)
			list = list->next;
	}
	return (SUCCESS);
}

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
	while (tab && tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = NULL;
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
	return (epur_files(f, s, i));
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

int	create_new_file(t_file *f, char *file, int type, int token)
{
	int	i;

	i = 0;
	while (f->files[i])
		i++;
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
	file->files = NULL;
	file->modes = NULL;
	file->token = NULL;
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
	free(file->files);
	free(file->token);
	free(file->modes);
	s->infile = NULL;
	s->outfile = NULL;
	s->mode_in = NOTHING;
	s->mode_out = NOTHING;
	s->token_in = NOTHING;
	s->token_out = NOTHING;
	set_files(file);
}

int	epur_str(char *s1, t_struct **s, int *i)
{
	char	*str;

	str = expand(s1, (*s)->env, *s);
	if (!str)
		return ((*s)->tab[(*i)] = NULL, flush_array((*s)->tab), ERR_MALLOC);
	if (!str[0])
		return (free(str), (*i)--, SUCCESS);
	str = takeoff_quotes(str);
	if (!str)
		return ((*s)->tab[(*i)] = NULL, flush_array((*s)->tab), ERR_MALLOC);
	(*s)->tab[(*i)] = str;
	return (SUCCESS);
}

int	fill_cmd_tab(t_last_list **list, t_struct **s)
{
	int	i;

	i = 0;
	while ((*s)->tab[i])
		i++;
	if ((*list)->token == TK_END)
		return ((*s)->tab[i] = NULL, SUCCESS);
	while ((*list)->next)
	{
		if (token_redirection((*list)->token, 3))
			break ;
		if ((*list)->str)
		{
			if (epur_str((*list)->str, s, &i) != SUCCESS)
				return (ERR_MALLOC);
		}
		i++;
		(*list) = (*list)->next;
	}
	(*s)->tab[i] = NULL;
	return (SUCCESS);
}

// static t_last_list	*new_node(void)
// {
// 	t_last_list	*list;

// 	list = malloc(sizeof(t_last_list));
// 	if (!list)
// 		return (NULL);
// 	list->next = NULL;
// 	list->prev = NULL;
// 	list->token = -100;
// 	return (list);
// }

// static int	create(t_last_list **list)
// {
// 	if ((*list)->token == -100 || !(*list)->str)
// 	{
// 		if ((*list)->str)
// 			free((*list)->str);
// 		(*list)->str = NULL;
// 		return (SUCCESS);
// 	}
// 	(*list)->next = new_node();
// 	if (!(*list)->next)
// 		return (ERR_MALLOC);
// 	(*list)->next->prev = (*list);
// 	(*list) = (*list)->next;
// 	(*list)->str = NULL;
// 	return (SUCCESS);
// }

int	check_buildin(t_struct *s)
{
	if (ft_strcmp(s->tab[0], "exit") == 0)
		return (0);
	else if (ft_strcmp(s->tab[0], "export") == 0)
		return (0);
	else if (ft_strcmp(s->tab[0], "unset") == 0)
		return (0);
	return (1);
}

int	execute_buildin_normal(t_struct *s)
{
	if (ft_strcmp(s->tab[0], "exit") == 0)
		return (ft_exit(s), EXIT);
	else if (ft_strcmp(s->tab[0], "export") == 0)
		return (ft_export_notchild(s, s->dup_env), NORMAL);
	else if (ft_strcmp(s->tab[0], "unset") == 0)
		return (ft_unset(s, s->dup_env), NORMAL);
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
	if ((*s)->tab[0] && check_buildin(*s) == 0 && (*s)->count_pipes == 0)
		return (execute_buildin_normal(*s));
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

void	get_size_files(t_last_list **list, int *size_files)
{
	while ((*list)->next && (*list)->next->str
		&& token_redirection((*list)->token, 0))
	{
		(*size_files)++;
		(*list) = (*list)->next->next;
	}
}

int	get_size_tab(t_last_list *list, int *size_tab, int *size_files)
{
	if (list->token == TK_END)
		return (SUCCESS);
	while (list->next)
	{
		if (token_redirection(list->token, 0))
			get_size_files(&list, &(*size_files));
		if (list->token == TK_END || list->token == TK_PIPES)
			break ;
		if (list->str)
			(*size_tab)++;
		list = list->next;
	}
	return (SUCCESS);
}

int	recursive_filler(t_struct **s, t_last_list **list, t_file *file)
{
	int	size_tab;
	int	size_files;

	size_tab = 0;
	size_files = 0;
	get_size_tab((*list), &size_tab, &size_files);
	file->files = malloc(sizeof(char *) * (size_files + 1));
	if (!file->files)
		return (ERR_MALLOC);
	file->modes = malloc(sizeof(int) * (size_files + 1));
	if (!file->modes)
		return (free(file->files), ERR_MALLOC);
	file->token = malloc(sizeof(int) * (size_files + 1));
	if (!file->token)
		return (free(file->files), free(file->modes), ERR_MALLOC);
	(*s)->tab = malloc(sizeof(char *) * (size_tab + 1));
	if (!(*s)->tab)
		return (free(file->files), free(file->modes), free(file->token),
			ERR_MALLOC);
	(*s)->tab[0] = NULL;
	file->files[0] = NULL;
	file->modes[0] = NOTHING;
	file->token[0] = NOTHING;
	return (recursive_tab_filler(&(*s), &(*list), &(*file)));
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
	(void)pipe;
	while (list->next)
	{
		if (is_arg(list->token) || token_redirection(list->token, 0))
		{
			status = recursive_filler(&s, &list, &file);
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
		if (list->next)
			list = list->next;
	}
	return (SUCCESS);
}

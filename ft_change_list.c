/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_change_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 16:12:37 by thole             #+#    #+#             */
/*   Updated: 2024/02/03 16:12:38 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static t_last_list	*new_node(int size)
{
	t_last_list	*list;

	(void)size;
	list = malloc(sizeof(t_last_list));
	if (!list)
		return (NULL);
	list->next = NULL;
	list->prev = NULL;
	list->token = -100;
	return (list);
}

static t_last_list	*new_list(void)
{
	t_last_list	*list;

	list = malloc(sizeof(t_last_list));
	if (!list)
		return (NULL);
	list->next = NULL;
	list->str = NULL;
	list->prev = NULL;
	list->token = -100;
	return (list);
}

static t_parse_list	*skip_first(t_parse_list *copy)
{
	while (copy->next && copy->token == TK_SKIP)
	{
		copy = copy->next;
	}
	return (copy);
}

void	char_to_string(char c, char *str)
{
	str[0] = c;
	str[1] = '\0';
	return ;
}

int	check_if_arg(t_struct *parse)
{
	if (parse->p_lst->next && (parse->p_lst->token == TK_ARG
			|| parse->p_lst->token == TK_WILD)
		&& parse->p_lst->token != TK_SKIP)
		return (1);
	return (0);
}

t_last_list	*step(t_last_list **list)
{
	if ((*list)->token == -100)
	{
		if ((*list)->str)
			free((*list)->str);
		(*list)->str = NULL;
		return (*list);
	}
	(*list)->next = new_node(0);
	if (!(*list)->next)
		return (NULL);
	(*list)->next->prev = (*list);
	(*list) = (*list)->next;
	(*list)->str = NULL;
	return (*list);
}

t_last_list	*create(t_last_list **list, int *size)
{
	if ((*list)->token == -100 || !(*list)->str)
	{
		if ((*list)->str)
			free((*list)->str);
		(*list)->str = calloc(sizeof(char), (*size) + 1);
		if (!(*list)->str)
			return (NULL);
		(*size) = 0;
		return (*list);
	}
	(*list)->next = new_node(*size);
	if (!(*list)->next)
		return (NULL);
	(*list)->next->prev = (*list);
	(*list) = (*list)->next;
	(*list)->str = calloc(sizeof(char), (*size) + 1);
	if (!(*list)->str)
		return (NULL);
	(*size) = 0;
	return (*list);
}

int	get_token(t_struct **parse, t_last_list **list)
{
	int	token;

	if ((*parse)->p_lst->token != TK_APPEND
		&& (*parse)->p_lst->token != TK_DLMTR
		&& (*parse)->p_lst->token != TK_EXPAND
		&& (*parse)->p_lst->token != TK_AND && (*parse)->p_lst->token != TK_OR)
	{
		token = (*parse)->p_lst->token;
		(*list)->str[0] = (*parse)->p_lst->c;
		(*list)->str[1] = '\0';
		if ((*parse)->p_lst->next)
			(*parse)->p_lst = (*parse)->p_lst->next;
		return (token);
	}
	token = (*parse)->p_lst->token;
	(*list)->str[0] = (*parse)->p_lst->c;
	(*parse)->p_lst = (*parse)->p_lst->next;
	(*list)->str[1] = (*parse)->p_lst->c;
	(*parse)->p_lst = (*parse)->p_lst->next;
	(*list)->str[2] = '\0';
	return (token);
}

int	check_argument(int token)
{
	if ((token == TK_ARG || token == TK_WILD) && token != TK_SKIP
		&& token != TK_END)
		return (1);
	return (0);
}

int	next_stop_token(int token)
{
	if ((token == TK_PIPES || token == TK_APPEND || token == TK_DLMTR
			|| token == TK_INPUT || token == TK_OUTPUT || token == TK_END
			|| token == TK_END_PRIO || token == TK_PRIO))
		return (1);
	return (0);
}

int	quote_line_fill(t_struct **parse, t_last_list **list, int type, int *size)
{
	char	*str;
	int		delimiter;

	delimiter = *size;
	(*size)++;
	*list = create(&(*list), &(*size));
	if (!(*list))
		return (ERR_MALLOC);
	while ((*parse)->p_lst->next && (*size) < delimiter)
	{
		str = malloc(sizeof(char) * 2);
		if (!str)
			return (ERR_MALLOC);
		char_to_string((*parse)->p_lst->c, str);
		ft_memcpy((*list)->str + (*size), str, 1);
		if ((*list)->token == -100)
			(*list)->token = type;
		(*size)++;
		(*parse)->p_lst = (*parse)->p_lst->next;
		free(str);
	}
	return (SUCCESS);
}

int	recursive_line_size(t_struct **parse, int *type, int *size)
{
	if ((*parse)->p_lst->token == TK_SINGLE
		|| (*parse)->p_lst->token == TK_DOUBLE)
		(*type) = (*parse)->p_lst->token;
	(*parse)->p_lst = (*parse)->p_lst->next;
	if ((*parse)->p_lst->token == (*type))
	{
		(*size)++;
		(*parse)->p_lst = (*parse)->p_lst->next;
		while ((*parse)->p_lst->next && (*parse)->p_lst->token != TK_END
			&& check_argument((*parse)->p_lst->token))
		{
			(*size)++;
			(*parse)->p_lst = (*parse)->p_lst->next;
		}
		(*size)++;
	}
	else
	{
		while ((*parse)->p_lst->next && (*parse)->p_lst->token != TK_END
			&& (*parse)->p_lst->token != (*type))
		{
			(*size)++;
			(*parse)->p_lst = (*parse)->p_lst->next;
		}
		if ((*parse)->p_lst->next)
		{
			(*size)++;
			((*parse)->p_lst = (*parse)->p_lst->next);
		}
		while ((*parse)->p_lst->next && (*parse)->p_lst->token != TK_END
			&& check_argument((*parse)->p_lst->token))
		{
			(*size)++;
			(*parse)->p_lst = (*parse)->p_lst->next;
		}
		if (check_argument((*parse)->p_lst->token))
			(*size)++;
	}
	if ((*parse)->p_lst->next && ((*parse)->p_lst->token == TK_DOUBLE
			|| (*parse)->p_lst->token == TK_SINGLE))
		return (recursive_line_size(&(*parse), &(*type), &(*size)));
	return (SUCCESS);
}

int	ft_change_list(t_struct *parse)
{
	t_last_list		*list;
	t_last_list		*head;
	t_parse_list	*copy;
	int				size;
	char			*str;
	int				type;

	type = 0;
	head = new_list();
	if (!head)
		return (ERR_MALLOC);
	list = head;
	copy = parse->p_lst;
	while (parse->p_lst->next)
	{
		size = 0;
		if (parse->p_lst->next)
			parse->p_lst = skip_first(parse->p_lst);
		copy = parse->p_lst;
		while (check_if_arg(parse) && (!(next_stop_token(parse->p_lst->token))))
		{
			size++;
			if (list->token != TK_SINGLE && list->token != TK_DOUBLE
				&& (parse->p_lst->token == TK_SINGLE
					|| parse->p_lst->token == TK_DOUBLE) && !list->str)
				list->token = parse->p_lst->token;
			else if (list->token == -100)
				list->token = parse->p_lst->token;
			parse->p_lst = parse->p_lst->next;
		}
		if (parse->p_lst)
		{
			list = create(&list, &size);
			if (!list)
				return (ERR_MALLOC);
			parse->p_lst = copy;
			if (parse->p_lst->next)
				parse->p_lst = skip_first(parse->p_lst);
			while (check_if_arg(parse)
				&& (!(next_stop_token(parse->p_lst->token))))
			{
				str = malloc(sizeof(char) * 2);
				if (!str)
					return (ERR_MALLOC);
				char_to_string(parse->p_lst->c, str);
				ft_memcpy(list->str + size, str, 1);
				if (list->token != TK_SINGLE && list->token != TK_DOUBLE
					&& (parse->p_lst->token == TK_SINGLE
						|| parse->p_lst->token == TK_DOUBLE))
					list->token = parse->p_lst->token;
				else if (list->token == -100)
					list->token = parse->p_lst->token;
				size++;
				parse->p_lst = parse->p_lst->next;
				free(str);
			}
			if (parse->p_lst->token == TK_SINGLE
				|| parse->p_lst->token == TK_DOUBLE)
			{
				size = 1;
				type = parse->p_lst->token;
				copy = parse->p_lst;
				recursive_line_size(&parse, &type, &size);
				parse->p_lst = copy;
				if (check_argument(parse->p_lst->prev->token))
				{
					list->token = -100;
					while (check_argument(parse->p_lst->prev->token))
					{
						parse->p_lst = parse->p_lst->prev;
						size++;
					}
				}
				if (quote_line_fill(&parse, &list, type, &size) == ERR_MALLOC)
					return (ft_free_changed_list(head), ERR_MALLOC);
			}
			if (parse->p_lst)
				parse->p_lst = skip_first(parse->p_lst);
			if (parse->p_lst && next_stop_token(parse->p_lst->token))
			{
				list = step(&list);
				if (!list)
					return (ERR_MALLOC);
				list->str = calloc(sizeof(char), 3);
				if (!list->str)
					return (ERR_MALLOC);
				list->token = get_token(&parse, &list);
			}
			if (parse->p_lst)
				parse->p_lst = skip_first(parse->p_lst);
		}
	}
	if (list->token != TK_END)
	{
		list->next = new_list();
		if (!list->next)
			return (ERR_MALLOC);
		list->next->prev = list;
		list->next->str = NULL;
		list = list->next;
		list->next = NULL;
		list->str = NULL;
		list->token = parse->p_lst->token;
	}
	list = head;
	parse->l_lst = list;
	return (SUCCESS);
}

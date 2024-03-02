/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   condition.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 13:06:45 by thole             #+#    #+#             */
/*   Updated: 2024/02/10 13:06:46 by thole            ###   ########.fr       */
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
	list->token = -1000;
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
	list->token = -1000;
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
			|| parse->p_lst->token == TK_WILD
			|| parse->p_lst->token == TK_SINGLE
			|| parse->p_lst->token == TK_DOUBLE)
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

int	get_token(t_struct **parse)
{
	int	token;

	if ((*parse)->p_lst->token != TK_APPEND
		&& (*parse)->p_lst->token != TK_DLMTR
		&& (*parse)->p_lst->token != TK_EXPAND
		&& (*parse)->p_lst->token != TK_AND && (*parse)->p_lst->token != TK_OR)
	{
		token = (*parse)->p_lst->token;
		if ((*parse)->p_lst->next)
			(*parse)->p_lst = (*parse)->p_lst->next;
		return (token);
	}
	token = (*parse)->p_lst->token;
	(*parse)->p_lst = (*parse)->p_lst->next;
	(*parse)->p_lst = (*parse)->p_lst->next;
	return (token);
}

int	next_stop_token(int token)
{
	if ((token == TK_PIPES || token == TK_APPEND || token == TK_DLMTR
			|| token == TK_INPUT || token == TK_OUTPUT || token == TK_END)
		&& token != TK_SKIP)
		return (1);
	return (0);
}

int	loop_string_maker_first_part(t_struct **parse, t_last_list **list)
{
	char	*str;
	int		size;

	size = 0;
	while (check_if_arg(*parse) && (!(next_stop_token((*parse)->p_lst->token))))
	{
		str = malloc(sizeof(char) * 2);
		if (!str)
			return (ERR_MALLOC);
		char_to_string((*parse)->p_lst->c, str);
		ft_memcpy((*list)->str + size, str, 1);
		(*list)->token = (*parse)->p_lst->token;
		size++;
		(*parse)->p_lst = (*parse)->p_lst->next;
		free(str);
	}
	return (SUCCESS);
}

int	first_part(t_struct **parse, t_last_list **list, t_parse_list **copy,
		int size)
{
	(*parse)->p_lst = skip_first((*parse)->p_lst);
	*copy = (*parse)->p_lst;
	while (check_if_arg((*parse))
		&& (!(next_stop_token((*parse)->p_lst->token))))
	{
		size++;
		(*parse)->p_lst = (*parse)->p_lst->next;
	}
	(*list)->str = calloc(sizeof(char), (size) + 1);
	if (!(*list)->str)
		return (ERR_MALLOC);
	(*parse)->p_lst = *copy;
	size = 0;
	if (loop_string_maker_first_part(&(*parse), &(*list)) == ERR_MALLOC)
		return (ERR_MALLOC);
	if (next_stop_token((*parse)->p_lst->token))
	{
		(*list) = step(&(*list));
		if (!(*list))
			return (ERR_MALLOC);
		(*list)->token = get_token(&(*parse));
	}
	return (SUCCESS);
}

int	checker_next_token(t_struct **parse, t_last_list **list)
{
	if ((*parse)->p_lst->next)
		(*parse)->p_lst = skip_first((*parse)->p_lst);
	if (next_stop_token((*parse)->p_lst->token))
	{
		(*list) = step(&(*list));
		if (!(*list))
			return (ERR_MALLOC);
		(*list)->token = get_token(&(*parse));
	}
	if ((*parse)->p_lst->next)
		(*parse)->p_lst = skip_first((*parse)->p_lst);
	return (SUCCESS);
}

int	loop_string_maker_second_part(t_struct **parse, t_last_list **list,
		t_parse_list **copy, int *size)
{
	char	*str;

	(*list) = create(&(*list), &(*size));
	if (!(*list))
		return (ERR_MALLOC);
	(*parse)->p_lst = *copy;
	if ((*parse)->p_lst->next)
		(*parse)->p_lst = skip_first((*parse)->p_lst);
	while (check_if_arg((*parse))
		&& (!(next_stop_token((*parse)->p_lst->token))))
	{
		str = malloc(sizeof(char) * 2);
		if (!str)
			return (ERR_MALLOC);
		char_to_string((*parse)->p_lst->c, str);
		ft_memcpy((*list)->str + *size, str, 1);
		(*list)->token = (*parse)->p_lst->token;
		(*size)++;
		(*parse)->p_lst = (*parse)->p_lst->next;
		free(str);
	}
	if (checker_next_token(&(*parse), &(*list)) == ERR_MALLOC)
		return (ERR_MALLOC);
	return (SUCCESS);
}

int	second_part(t_struct **parse, t_last_list **list, t_parse_list **copy)
{
	int	size;

	while ((*parse)->p_lst->next)
	{
		size = 0;
		if ((*parse)->p_lst->next)
			(*parse)->p_lst = skip_first((*parse)->p_lst);
		*copy = (*parse)->p_lst;
		while (check_if_arg((*parse))
			&& (!(next_stop_token((*parse)->p_lst->token))))
		{
			size++;
			(*parse)->p_lst = (*parse)->p_lst->next;
		}
		if ((*parse)->p_lst)
		{
			if (loop_string_maker_second_part(&(*parse), &(*list), &(*copy),
					&size) == ERR_MALLOC)
				return (ERR_MALLOC);
		}
	}
	return (SUCCESS);
}

int	last_part(t_last_list **list, t_struct **parse)
{
	if ((*list)->token != TK_END)
	{
		(*list)->next = new_list();
		if (!(*list)->next)
			return (ERR_MALLOC);
		(*list)->next->prev = (*list);
		(*list)->next->str = NULL;
		(*list) = (*list)->next;
		(*list)->token = (*parse)->p_lst->token;
	}
	return (SUCCESS);
}

int	ft_change_list(t_struct *parse)
{
	t_last_list		*list;
	t_last_list		*head;
	t_parse_list	*copy;
	int				size;

	head = new_list();
	if (!head)
		return (ERR_MALLOC);
	list = head;
	copy = parse->p_lst;
	size = 0;
	if (first_part(&parse, &list, &copy, size) == ERR_MALLOC)
		return (ft_free_changed_list(head), ERR_MALLOC);
	if (second_part(&parse, &list, &copy) == ERR_MALLOC)
		return (ft_free_changed_list(head), ERR_MALLOC);
	if (last_part(&list, &parse) == ERR_MALLOC)
		return (ft_free_changed_list(head), ERR_MALLOC);
	list = head;
	parse->l_lst = list;
	return (SUCCESS);
}

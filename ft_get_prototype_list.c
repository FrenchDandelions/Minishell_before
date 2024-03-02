/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 13:34:50 by thole             #+#    #+#             */
/*   Updated: 2024/02/03 13:34:52 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static int	type_token(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\r'
		|| c == '\f')
		return (TK_SKIP);
	else if (c == '(')
		return (TK_PRIO);
	else if (c == '*')
		return (TK_WILD);
	else if (c == '\'')
		return (TK_SINGLE);
	else if (c == '\"')
		return (TK_DOUBLE);
	else if (c == '<')
		return (TK_INPUT);
	else if (c == '>')
		return (TK_OUTPUT);
	else if (c == '|')
		return (TK_PIPES);
	else if (c == ')')
		return (TK_END_PRIO);
	else if (c == '&')
		return (TEMP_AND);
	return (TK_ARG);
}

static t_parse_list	*new_lst(char content, int token)
{
	t_parse_list	*lst;

	lst = malloc(sizeof(t_parse_list));
	if (!lst)
		return (NULL);
	lst->c = content;
	lst->token = token;
	lst->next = NULL;
	lst->prev = NULL;
	return (lst);
}

static t_parse_list	*change_token(t_parse_list **lst, char c, int new_tk,
		int *i)
{
	(*lst)->token = new_tk;
	(*lst)->next = new_lst(c, new_tk);
	if (!(*lst)->next)
		return (NULL);
	(*lst)->next->prev = (*lst);
	(*lst) = (*lst)->next;
	(*i)++;
	return (*lst);
}

static t_parse_list	*check_next(t_parse_list **lst, char c, int *i)
{
	if (!c)
		return (*lst);
	if (c == '<' && (*lst)->c == '<')
		*lst = change_token(&(*lst), c, TK_DLMTR, &(*i));
	else if (c == '>' && (*lst)->c == '>')
		*lst = change_token(&(*lst), c, TK_APPEND, &(*i));
	else if (c == '|' && (*lst)->c == '|')
		*lst = change_token(&(*lst), c, TK_OR, &(*i));
	else if (c == '&' && (*lst)->c == '&')
		*lst = change_token(&(*lst), c, TK_AND, &(*i));
	return (*lst);
}

int	ft_prototype_list(t_struct *s)
{
	t_parse_list	*lst;
	t_parse_list	*head;
	int				i;

	i = 0;
	if (!(s->str))
		return (NOTHING);
	head = new_lst(s->str[i], type_token(s->str[i]));
	if (!head)
		return (ERR_MALLOC);
	lst = head;
	while (s->str[i++])
	{
		lst = check_next(&lst, s->str[i], &i);
		if (!lst)
			return (ft_free_parse_list(head), ERR_MALLOC);
		lst->next = new_lst(s->str[i], type_token(s->str[i]));
		if (!lst->next)
			return (ft_free_parse_list(head), ERR_MALLOC);
		lst->next->prev = lst;
		lst = lst->next;
	}
	lst->next = NULL;
	lst->token = TK_END;
	return (s->p_lst = head, SUCCESS);
}

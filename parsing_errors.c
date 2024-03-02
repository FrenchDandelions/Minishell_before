/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:54:12 by thole             #+#    #+#             */
/*   Updated: 2024/02/23 12:54:13 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static int	is_pipe(int token, int i)
{
	if (i == 1 && (token == TK_PIPES || token == TK_AND))
		return (1);
	else if (i == 0 && (token == TK_PIPES || token == TK_AND || token == TK_OR))
		return (1);
	else if (i == 2 && (token == TK_ARG || token == TK_SINGLE
			|| token == TK_DOUBLE))
		return (1);
	return (0);
}

static int	is_redirection(int token, int i)
{
	if (i == 0)
	{
		if (token == TK_INPUT || token == TK_APPEND || token == TK_DLMTR
			|| token == TK_OUTPUT || token == TK_AND || token == TEMP_AND)
			return (1);
	}
	else
	{
		if (token == TK_INPUT || token == TK_APPEND || token == TK_DLMTR
			|| token == TK_OUTPUT || token == TK_AND || token == TEMP_AND
			|| token == TK_PIPES || token == TK_OR)
			return (1);
	}
	return (0);
}

static int	is_special(int token)
{
	if (token == TK_INPUT || token == TK_APPEND || token == TK_DLMTR
		|| token == TK_OUTPUT || token == TK_AND || token == TEMP_AND
		|| token == TK_PIPES || token == TK_AND)
		return (1);
	return (0);
}

int	print_error(char *str)
{
	if (str)
		printf("Minishell: syntax error near unexpected token '%s'\n", str);
	else
		printf("Minishell: syntax error near unexpected token 'newline'\n");
	return (ERR_PARS);
}

int	check_heredoc(t_last_list **list)
{
	int	err;

	if ((*list)->token == TK_DLMTR)
	{
		if (!(*list)->next && (*list)->token == TK_DLMTR)
			return (ERR_PARS);
		else if ((*list)->next && ((*list)->next->token == TK_ARG
				|| (*list)->next->token == TK_DOUBLE
				|| (*list)->next->token == TK_SINGLE)
			&& (*list)->token == TK_DLMTR)
		{
			err = open_heredoc(&(*list));
			(*list) = (*list)->next;
			return (err);
		}
	}
	return (SUCCESS);
}

int	parse_heredoc(t_struct *s)
{
	t_last_list	*temp;
	int			err;

	temp = s->head_ll;
	while (temp->next)
	{
		err = check_heredoc(&temp);
		if (err == ERR_PARS)
			return (print_error("<<"));
		else if (err == ERR_MALLOC)
			return (ERR_MALLOC);
		temp = temp->next;
	}
	return (SUCCESS);
}

int	parser(t_struct *s)
{
	t_last_list	*temp;

	temp = s->head_ll;
	while (temp->next)
	{
		if (!temp->prev && is_pipe(temp->token, 0))
			return (print_error(temp->str));
		else if (temp->prev && (is_pipe(temp->prev->token, 2)
				&& temp->token == TK_PRIO))
			return (print_error(temp->str));
		else if (temp->next->token == TK_END && is_special(temp->token))
			return (print_error(NULL));
		else if (temp->next && is_redirection(temp->token, 0)
			&& is_redirection(temp->next->token, 1))
			return (print_error(temp->next->str));
		else if (temp->next && is_pipe(temp->token, 1)
			&& is_pipe(temp->next->token, 1))
			return (print_error(temp->next->str));
		temp = temp->next;
	}
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:43:51 by thole             #+#    #+#             */
/*   Updated: 2024/03/12 14:43:53 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	count_pipes(t_struct *s)
{
	t_last_list	*temp;

	temp = s->head_ll;
	while (temp->next)
	{
		if (temp->token == TK_PIPES)
			s->count_pipes++;
		temp = temp->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   go_to_next_stop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 13:18:07 by thole             #+#    #+#             */
/*   Updated: 2024/03/06 13:18:08 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	go_to_next_stop(t_last_list **list)
{
	int	prio;

	prio = 0;
	while ((*list) && (*list)->next)
	{
		if ((*list)->token == TK_PRIO)
			prio++;
		if (prio == 1)
		{
			while (prio != 0 && (*list)->next)
			{
				if ((*list)->token == TK_PRIO)
					prio++;
				else if ((*list)->token == TK_END_PRIO)
					prio--;
				(*list) = (*list)->next;
			}
		}
		if ((*list)->token == TK_PIPES || (*list)->token == TK_END)
			break ;
		if ((*list))
			(*list) = (*list)->next;
	}
	return (SUCCESS);
}

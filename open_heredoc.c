/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:07:17 by thole             #+#    #+#             */
/*   Updated: 2024/02/27 17:07:18 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	end_heredoc(t_last_list **list, char *buf, char *str)
{
	free(str);
	free((*list)->next->str);
	if (buf)
	{
		(*list)->next->str = strdup(buf);
		if (!(*list)->next->str)
			return (free(buf), ERR_MALLOC);
		free(buf);
	}
	else
	{
		(*list)->next->str = ft_strdup("");
		if (!(*list)->next->str)
			return (ERR_MALLOC);
	}
	return (SUCCESS);
}

int	add_to_buffer(char **buf, char *str)
{
	if ((*buf))
	{
		(*buf) = ft_gnl_strjoin((*buf), str, (int)strlen(str));
		if (!(*buf))
			return (ERR_MALLOC);
	}
	else
	{
		(*buf) = strdup(str);
		if (!(*buf))
			return (free(str), ERR_MALLOC);
	}
	return (SUCCESS);
}

int	open_heredoc(t_last_list **list)
{
	char	*str;
	char	*buf;

	buf = NULL;
	while (1)
	{
		str = readline("\033[38;5;220mmini_doc>\033[0m ");
		if (!str)
			return (dprintf(STDERR_FILENO, "%s%s')\n\033[0m", ERR_MINI_DOC,
					(*list)->next->str), end_heredoc(&(*list), buf, str));
		if (strncmp(str, (*list)->next->str, (int)ft_strlen(str)) == 0)
			return (end_heredoc(&(*list), buf, str));
		str = ft_gnl_strjoin(str, "\n", 1);
		if (add_to_buffer(&buf, str) == ERR_MALLOC)
			return (ERR_MALLOC);
		free(str);
	}
}

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
	if ((int)strlen(buf) > 0)
	{
		(*list)->next->str = strdup(buf);
		if (!(*list)->next->str)
			return (ERR_MALLOC);
		free(buf);
	}
	else
	{
		free(buf);
		(*list)->next->str = NULL;
	}
	return (SUCCESS);
}

int	realloc_buffer(char **buf, int *size, char *str)
{
	(*size) = (*size) * 2;
	(*buf) = realloc((*buf), (*size));
	if (!(*buf))
		return (free(str), ERR_MALLOC);
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
			return (ERR_MALLOC);
	}
	return (SUCCESS);
}

int	open_heredoc(t_last_list **list)
{
	char	*str;
	char	*buf;
	int		size;

	size = SIZE;
	buf = strdup("");
	if (!buf)
		return (ERR_MALLOC);
	while (1)
	{
		str = readline("\033[38;5;220mmini_doc>\033[0m ");
		if (!str)
			return (dprintf(STDERR_FILENO, "%s%s')\n\033[0m", ERR_MINI_DOC,
					(*list)->next->str), end_heredoc(&(*list), buf, str));
		str = ft_gnl_strjoin(str, "\n", 1);
		if (buf && ((int)strlen(buf) + (int)strlen(str)) > size)
			if (realloc_buffer(&buf, &size, str) == ERR_MALLOC)
				return (ERR_MALLOC);
		if (strncmp(str, (*list)->next->str,
				(int)strlen((*list)->next->str)) == 0)
			return (end_heredoc(&(*list), buf, str));
		if (add_to_buffer(&buf, str) == ERR_MALLOC)
			return (ERR_MALLOC);
		free(str);
	}
}
